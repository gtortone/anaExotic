#ifndef ROOTANA_EXOTIC_H
#define ROOTANA_EXOTIC_H
#include "utils/Exotic.h"
#include "rootana/Directory.h"
#include <TApplication.h>
#include <THttpServer.h>
#include <list>
#include <memory>

class TFile;
class TDirectory;

namespace midas {
class Event;
class Database;
} // namespace midas

namespace tstamp {
class Queue;
class Diagnostics;
} // namespace tstamp

namespace rootana {

class MidasOnline;

/// Application class for exotic rootana
class App : public TApplication {

   /// Tells the running mode of the application
   enum Mode_t { ONLINE,
                 OFFLINE };

 private:
   int fRunNumber;                                         ///< Current run number
   int fMode;                                              ///< Running mode (online or offline)
   int fCutoff;                                            ///< Event cutoff (offline only)
   int fReturn;                                            ///< Return value
   int fTcp;                                               ///< TCP port value
   int fHttp;                                              ///< HTTP port value
   double fCoincWindow;                                    ///< Coincidence window for timestamping
   std::string fFilename;                                  ///< Offline file name
   std::string fHost;                                      ///< Online host name
   std::string fExpt;                                      ///< Online experiment name
   std::string fHistos;                                    ///< Histogram specification file (online + file)
   std::string fDetectors;                                 ///< Detectors specification file (json)
   std::string fHistosOnline;                              ///< Histogram specification file (online only)
   THttpServer *fHttpServ;                                 ///< HTTP server handler
   std::unique_ptr<rootana::OfflineDirectory> fOutputFile; ///< Online/offline histograms
   std::unique_ptr<rootana::OnlineDirectory> fOnlineHists; ///< Online-only histograms
   std::unique_ptr<midas::Database> fOdb;                  ///< Online/offline database
   std::unique_ptr<MidasOnline> fMidasOnline;              ///< "Online midas" instance

 public:
   /// Calls TApplication constructor
   App(char const *appClassName, int *argc, char **argv);

   /// Return gApplication cast to rootana::App
   static App *instance();

   /// Runs the application
   void Run(Bool_t retrn = kFALSE);

   /// Terminates the application
   void Terminate(Int_t status = 0);

   /// Gets fReturn
   int ReturnVal() { return fReturn; }

   //  /// Start-of-run actions
   void run_start(int runnum);

   //  /// End-of-run acitons
   void run_stop(int runnum);

   /// Handle a midas event
   void handle_event(midas::Event &event);

   /// Tells how to handle a singles event from the beginning of fQueue
   void Process(const midas::Event &event);

   /// Process an offline MIDAS file
   int midas_file(const char *fname);

   /// Process online MIDAS data
   int midas_online(const char *host = "", const char *experiment = "exotic");

   /// Deletes fQueue if it's non-null;
   ~App();

 private:
   /// Prints 'help' message
   void help();

   /// Processes command line args
   void process_argv(int argc, char **argv);

   /// Handles cleanup for destructor or Terminate()
   void do_exit();

   /// Fills all histos w/ a specific event ID
   void fill_hists(uint16_t eid);

   /// Process detectors JSON file
   void process_json(void);

   ClassDef(rootana::App, 0);     // rootana
};

} // namespace rootana

#endif
