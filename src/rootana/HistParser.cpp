#include "rootana/HistParser.h"
#include "midas.h"
#include "rootana/Directory.h"
#include "rootana/Histos.h"
#include "rootana/DetectorContainer.h"
#include "utils/ErrorExotic.h"
#include <TDirectory.h>
#include <TInterpreter.h>
#include <TROOT.h>
#include <TError.h>
#include <algorithm>
#include <sstream>
#include <stdexcept>

// HELPER FUNCTIONS & CLASSES //

/// Helper macro
#define CHECK_ERROR(code)                                \
   do {                                                  \
      if (code != TInterpreter::EErrorCode::kNoError) {  \
         Error("HistParser", "Failed on line %i", __LINE__); \
      }                                                  \
   } while (false)

namespace {

struct replace_tab {
   void operator()(char &c) {
      if (c == '\t')
         c = ' ';
   }
};

inline bool contains(const std::string &s, const char *arg) {
   return s.find(arg) < s.size();
}

inline void throw_bad_line(const std::string &line, int linenum, const char *fname, std::stringstream *pcmd = 0) {
   std::stringstream error;
   error << "Bad line in file \"" << fname << "\": " << line << ", line number: " << linenum;
   if (pcmd)
      error << "\n(Cmd: " << pcmd->str() << " )";
   throw std::invalid_argument(error.str().c_str());
}

inline void throw_missing_arg(const char *which, int linenum, const char *fname) {
   std::stringstream error;
   error << "Missing \"" << which << "\" argument in file \"" << fname << "\" at line " << linenum;
   throw std::invalid_argument(error.str().c_str());
}

} // namespace

// HIST PARSER //

rootana::HistParser::HistParser(const char *filename) : fFilename(filename), fFile(filename),
                                                        fLine(""), fLineNumber(0), fDir("") {
   /*!
	 *  \param filename Path to the histogram definition file
	 *
	 *  Prints error message if file name is invalid
	 */
   if (!IsGood()) {
      std::stringstream error;
      error << "Invalid file path: " << filename;
      throw std::invalid_argument(error.str().c_str());
   }

   for (int i = 0; i < gROOT->GetListOfSpecials()->GetEntries(); ++i) {
      TCutG *cutg = dynamic_cast<TCutG *>(gROOT->GetListOfSpecials()->At(i));
      if (cutg)
         fExistingCuts.push_back(cutg);
   }
}

rootana::HistParser::~HistParser() {
   for (std::vector<TCutG *>::iterator it = fLocalCuts.begin(); it != fLocalCuts.end(); ++it) {
      (*it)->Delete();
   }
   std::list<HistInfo>::iterator itHist = fCreatedHistograms.begin();
   for (; itHist != fCreatedHistograms.end(); ++itHist) {
      if (itHist->fHist)
         delete itHist->fHist;
   }
}

bool rootana::HistParser::read_line() {
   /*!
	 * In addition to reading the line, does the following:
	 */
   std::getline(fFile, fLine);
   fLineNumber++;

   /// - Ignore everything after the first \c #
   size_t ppos = fLine.find('#');
   if (ppos < fLine.size())
      fLine = fLine.substr(0, ppos);

   /// - Replace each tab with a space
   std::for_each(fLine.begin(), fLine.end(), replace_tab());

   /// \returns false if at eof, true otherwise
   return IsGood();
}

void rootana::HistParser::handle_command() {
   bool done = false;
   int err;
   while (read_line()) {
      if (contains(fLine, "END:")) {
         done = true;
         break;
      }
      gROOT->ProcessLine(fLine.c_str(), &err);
      if (err != 0)
         throw_bad_line(fLine, fLineNumber, fFilename);
   }
   if (!done)
      throw_missing_arg("CMD:", fLineNumber, fFilename);

   for (int i = 0; i < gROOT->GetListOfSpecials()->GetEntries(); ++i) {
      TCutG *cutg = dynamic_cast<TCutG *>(gROOT->GetListOfSpecials()->At(i));
      if (cutg && std::count(fExistingCuts.begin(), fExistingCuts.end(), cutg) == 0) {
         fLocalCuts.push_back(cutg);
      }
   }
}

void rootana::HistParser::handle_dir() {
   read_line();
   if (!IsGood())
      throw_missing_arg("DIR:", fLineNumber, fFilename);
   fDir = fLine;
   std::cout << "\n";
   exotic::utils::Info("rootana::HistParser") << "New directory: " << fDir;
}

namespace {
template <class T>
T *create_hist(const char *type, std::string sHst, unsigned lHst, std::string filename) {
   std::stringstream cmd;
   cmd << "new " << type << sHst << ";";
   T *out = (T *)gROOT->ProcessLine(cmd.str().c_str());
   if (!out)
      throw_bad_line(sHst, lHst, filename.c_str(), &cmd);
   return out;
}
} // namespace

void rootana::HistParser::handle_hist(const char *type) {
   int npar = atoi(std::string(type).substr(2, 1).c_str());
   assert(npar > 0 && npar < 4);
   unsigned lhst, lpar[3];
   std::string shst, spar[3];
   {
      unsigned *lll[2] = {&lhst, lpar};
      std::string *lread[2] = {&shst, spar};

      read_line();
      if (!IsGood())
         throw_missing_arg("HIST:", fLineNumber, fFilename);
      *(lll[0]) = fLineNumber;
      *(lread[0]) = fLine;

      for (int i = 0; i < npar; ++i) {
         read_line();
         if (!IsGood())
            throw_missing_arg("HIST:", fLineNumber, fFilename);
         unsigned *lll1 = lll[1];
         lll1[i] = fLineNumber;
         std::string *lread1 = lread[1];
         lread1[i] = fLine;
      }
   }

   std::stringstream cmdParam[3];
   rootana::DataPointer *data[3];
   function_t func[3];
   bool hasFunc = false;

   if (spar[0].find("()") != std::string::npos)
      hasFunc = true;

   for (int i = 0; i < npar; ++i) {

      if (hasFunc) {

         std::cout << spar[i] << " is a function" << std::endl;
         std::string fname = exotic::utils::replaceString(spar[i], "()", "");
         TInterpreter::EErrorCode ecode;
         func[i] = (function_t)gInterpreter->Calc(std::string("&" + fname).c_str(), &ecode); // handle errors
         CHECK_ERROR(ecode);

      } else {

         cmdParam[i] << "rootana::DataPointer::New(" << spar[i] << ");";
         std::cout << "data[" << i << "] = " << cmdParam[i].str() << std::endl;
         data[i] = (rootana::DataPointer *)gROOT->ProcessLineFast(cmdParam[i].str().c_str());
         if (!data[i])
            throw_bad_line(spar[i], lpar[i], fFilename);
      }
   }

   // create histogram with parameter(s)
   if (!hasFunc) {

      rootana::HistBase *h = 0;
      switch (npar) {
      case 1:
         h = new rootana::HistParam<TH1D>(create_hist<TH1D>(type, shst, lhst, fFilename), data[0]);
         break;
      case 2:
         h = new rootana::HistParam<TH2D>(create_hist<TH2D>(type, shst, lhst, fFilename), data[0], data[1]);
         break;
      case 3:
         h = new rootana::HistParam<TH3D>(create_hist<TH3D>(type, shst, lhst, fFilename), data[0], data[1], data[2]);
         break;
      default: {
         std::stringstream err;
         err << "npar != 1, 2, or 3:: " __FILE__ << ", line " << __LINE__;
         throw std::invalid_argument(err.str().c_str());
      }
      }

      add_hist(h, -1);
   }

   // create histogram with function pointer(s)
   if (hasFunc) {

      rootana::HistBase *h = 0;
      switch (npar) {
      case 1:
         h = new rootana::HistFunc<TH1D>(create_hist<TH1D>(type, shst, lhst, fFilename), func[0]);
         break;
      case 2:
         h = new rootana::HistFunc<TH2D>(create_hist<TH2D>(type, shst, lhst, fFilename), func[0], func[1]);
         break;
      case 3:
         h = new rootana::HistFunc<TH3D>(create_hist<TH3D>(type, shst, lhst, fFilename), func[0], func[1], func[2]);
         break;
      default: {
         std::stringstream err;
         err << "npar != 1, 2, or 3:: " __FILE__ << ", line " << __LINE__;
         throw std::invalid_argument(err.str().c_str());
      }
      }

      add_hist(h, -1);
   }
}

void rootana::HistParser::handle_scaler() {
   unsigned lhst, lpar;
   std::string shst, spar;
   {
      unsigned *lll[2] = {&lhst, &lpar};
      std::string *lread[2] = {&shst, &spar};
      for (int i = 0; i < 2; ++i) {
         read_line();
         if (!IsGood())
            throw_missing_arg("SCALER:", fLineNumber, fFilename);
         *(lll[i]) = fLineNumber;
         *(lread[i]) = fLine;
      }
   }

   std::stringstream cmdData;
   cmdData << "rootana::DataPointer::New(" << spar << ");";
   rootana::DataPointer *data = (rootana::DataPointer *)gROOT->ProcessLineFast(cmdData.str().c_str());
   if (!data)
      throw_bad_line(spar, lpar, fFilename);

   std::stringstream cmdHist;
   cmdHist << "new TH1D" << shst << ";";
   TH1D *hst = (TH1D *)gROOT->ProcessLineFast(cmdHist.str().c_str());
   if (!hst)
      throw_bad_line(shst, lhst, fFilename);

   rootana::ScalerHist *h = new rootana::ScalerHist(hst, data);
   assert(h);

   add_hist(h, -1);
}

void rootana::HistParser::handle_summary() {
   unsigned lhst, lpar, lnum;
   std::string shst, spar, snum;
   {
      unsigned *lll[3] = {&lhst, &lpar, &lnum};
      std::string *lread[3] = {&shst, &spar, &snum};
      for (int i = 0; i < 3; ++i) {
         read_line();
         if (!IsGood())
            throw_missing_arg("SUMMARY:", fLineNumber, fFilename);
         *(lll[i]) = fLineNumber;
         *(lread[i]) = fLine;
      }
   }

   std::stringstream cmdData;
   cmdData << "rootana::DataPointer::New(" << spar << ", " << snum << ");";
   rootana::DataPointer *data = (rootana::DataPointer *)gROOT->ProcessLineFast(cmdData.str().c_str());
   if (!data)
      throw_bad_line(spar, lpar, fFilename);

   std::stringstream cmdHist;
   cmdHist << "new TH1D" << shst << ";";
   TH1D *hst = (TH1D *)gROOT->ProcessLineFast(cmdHist.str().c_str());
   if (!hst)
      throw_bad_line(shst, lhst, fFilename);

   rootana::SummaryHist *h = new rootana::SummaryHist(hst, data);
   assert(h);

   add_hist(h, -1);
}

void rootana::HistParser::handle_cut() {
   if (fCreatedHistograms.empty()) {
      std::stringstream err;
      err << "CUT: line without a prior histogram, in file " << fFilename << " at line " << fLineNumber;
      throw(std::invalid_argument(err.str().c_str()));
   }

   if (!read_line())
      throw_missing_arg("CUT:", fLineNumber, fFilename);
   std::stringstream cmd;
   cmd << "( " << fLine << " ).get()->clone();";
   rootana::Condition *condition = (rootana::Condition *)gROOT->ProcessLineFast(cmd.str().c_str());
   if (!condition)
      throw_bad_line(fLine, fLineNumber, fFilename, &cmd);

   std::list<HistInfo>::iterator itEnd = fCreatedHistograms.end();
   --itEnd;
   HistInfo *pInfo = &(*(itEnd));

   pInfo->fHist->set_cut(rootana::Cut(condition));

   std::cout << "\t\t";
   exotic::utils::Info("rootana::HistParser") << "Applying cut: " << fLine << " to histogram " << pInfo->fName;
}

void rootana::HistParser::add_hist(rootana::HistBase *hst, UShort_t type) {

   HistInfo histInfo = {hst, hst->name(), fDir, type};
   fCreatedHistograms.push_back(histInfo);

   std::stringstream tempName;
   tempName << "rootana__HistParser__temp___" << fCreatedHistograms.size();
   hst->set_name(tempName.str().c_str());

   std::cout << "\t";
   exotic::utils::Info("rootana::HistParser") << "Adding histogram " << histInfo.fName << " to directory " << fDir << ", EID = " << type;
}

void rootana::HistParser::Run() {
   
   gROOT->ProcessLine("using namespace rootana;");
   //gROOT->ProcessLine("extern DetectorContainer detList;");
   while (read_line()) {
      try {
         if (contains(fLine, "DIR:"))
            handle_dir();
         else if (contains(fLine, "CMD:"))
            handle_command();
         else if (contains(fLine, "CUT:"))
            handle_cut();
         else if (contains(fLine, "TH1D:"))
            handle_hist("TH1D");
         else if (contains(fLine, "TH2D:"))
            handle_hist("TH2D");
         else if (contains(fLine, "TH3D:"))
            handle_hist("TH3D");
         else if (contains(fLine, "SUMMARY:"))
            handle_summary();
         else if (contains(fLine, "SCALER:"))
            handle_scaler();
         else
            continue;
      } catch (std::exception &e) {
         cm_msg(MERROR, "anaExotic::HistParser::Run", "%s. Attempting to continue...", e.what());
         exotic::utils::Error("HistParser") << e.what() << "\nAttempting to continue...\n*******\n\n";
      }
   }
   std::cout << "\n";

   exotic::utils::Info("rootana::HistParser") << "Done creating histograms from file " << fFilename << std::endl;
}
