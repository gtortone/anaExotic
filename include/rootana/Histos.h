#ifndef EXOTIC_ROOTANA_HISTOS_H
#define EXOTIC_ROOTANA_HISTOS_H
#include "rootana/Cut.h"
#include "rootana/DataPointer.h"
#include "utils/Valid.h"
#include <TDirectory.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TROOT.h> // for gROOT
#include <TInterpreter.h>

typedef double (*function_t)(void);

/// \brief Encloses all rootana-specific classes
namespace rootana {

/// Abstract interface for Rootana histograms
/*!
 * Provides a pure virtual interface with functions needed in the standard
 * ROOTANA framework.
 * 
 * Also handles application of cuts for derived classes
 */
class HistBase {
 private:
   rootana::Cut fCut; ///< Cut (gate) condition

 public:
   /// Sets fCut to NULL, otherwise empty
   HistBase() : fCut(0) {}
   /// Empty
   virtual ~HistBase() {}
   /// Sets the cut (gate) condition
   void set_cut(const Cut &cut);
   /// Applies the cut condition
   bool apply_cut();

   /// Fills the histogram with appropriate data
   virtual Int_t fill() = 0;
   /// Draw the histogram
   virtual void draw(Option_t *option = "") const = 0;
   /// Writes the histogram to disk
   virtual void write() = 0;
   /// Clears the histogram
   virtual void clear() = 0;
   /// Sets histogram name
   virtual void set_name(const char *name) = 0;
   /// Returns histogram name
   virtual const char *name() const = 0;
   /// Sets owner TDirectory
   virtual void set_directory(TDirectory *) = 0;
   /// For testing
   void test() { printf("test\n"); }
};

/// Rootana histogram class
/*!
 * Wraps a normal TH*D, along with pointer(s) to the parameters with
 * which the histogram is filled. Dereference operators provide "smart pointer"
 * syntax.
 * \tparam T Type of ROOT histogram being wrapped. Only TH1D, TH2D, and TH3D
 * are allowed, and must be chosen with the right constructor.
 */
template <class T>
class HistParam : public HistBase {
 protected:
   T *fHist;                   ///< Internal ROOT histogram
   const DataPointer *fParamx; ///< X-axis parameter
   const DataPointer *fParamy; ///< Y-axis parameter
   const DataPointer *fParamz; ///< Z-axis parameter

   TDirectory *fHistOwner; /// Directory owning fHist

 public:
   /// 1d (TH1D) constructor (with parameter)
   HistParam(T *hist, const DataPointer *param);
   /// 2d (TH2D) constructor (with parameters)
   HistParam(T *hist, const DataPointer *paramx, const DataPointer *paramy);
   /// 3d (TH3D) constructor (with parameters)
   HistParam(T *hist, const DataPointer *paramx, const DataPointer *paramy, const DataPointer *paramz);

   /// Frees memory allocated to fHist, and fParamx (y, z)
   virtual ~HistParam();

 private:
   /// Copy constructor
   HistParam(const HistParam &other);
   /// Equivalency operator
   HistParam &operator=(const HistParam &other);

 public:
   /// Grants access to the internal histogram
   T *get() { return fHist; }
   /// Arrow operator, for pointer-like behavior
   T *operator->() { return fHist; }
   /// Dereference operator, for pointer-like bahavior
   T &operator*() { return *fHist; }

   /// Calls TH1::Fill using fParamx, fParamy, fParamz
   virtual Int_t fill();
   /// Calls TH1::Draw()
   virtual void draw(Option_t *option = "") const { fHist->Draw(option); }
   /// Calls TH1::Write()
   virtual void write() { fHist->Write(); }
   /// Calls TH1::SetName()
   virtual void set_name(const char *name) { fHist->SetName(name); }
   /// Calls TH1::GetName()
   virtual const char *name() const { return fHist->GetName(); }
   /// Calls TH1::Clear()
   virtual void clear() { fHist->Clear(); }
   /// Calls TH1::SetDirectory
   virtual void set_directory(TDirectory *directory) {
      fHist->SetDirectory(directory);
      fHistOwner = directory;
   }
};

template <class T>
class HistFunc : public HistBase {
 protected:
   T *fHist; ///< Internal ROOT histogram

   function_t fFuncx; ///< X-axis function
   function_t fFuncy; ///< Y-axis function
   function_t fFuncz; ///< Z-axis function

   TDirectory *fHistOwner; /// Directory owning fHist

 public:
   /// 1d (TH1D) constructor (with parameter)
   HistFunc(T *hist, const function_t func);
   /// 2d (TH2D) constructor (with parameters)
   HistFunc(T *hist, const function_t funcx, const function_t funcy);
   /// 3d (TH3D) constructor (with parameters)
   HistFunc(T *hist, const function_t funcx, const function_t funcy, const function_t funcz);

   /// Frees memory allocated to fHist, and fParamx (y, z)
   virtual ~HistFunc();

 private:
   /// Copy constructor
   HistFunc(const HistFunc &other);
   /// Equivalency operator
   HistFunc &operator=(const HistFunc &other);

 public:
   /// Grants access to the internal histogram
   T *get() { return fHist; }
   /// Arrow operator, for pointer-like behavior
   T *operator->() { return fHist; }
   /// Dereference operator, for pointer-like bahavior
   T &operator*() { return *fHist; }

   /// Calls TH1::Fill using fParamx, fParamy, fParamz
   virtual Int_t fill();
   /// Calls TH1::Draw()
   virtual void draw(Option_t *option = "") const { fHist->Draw(option); }
   /// Calls TH1::Write()
   virtual void write() { fHist->Write(); }
   /// Calls TH1::SetName()
   virtual void set_name(const char *name) { fHist->SetName(name); }
   /// Calls TH1::GetName()
   virtual const char *name() const { return fHist->GetName(); }
   /// Calls TH1::Clear()
   virtual void clear() { fHist->Clear(); }
   /// Calls TH1::SetDirectory
   virtual void set_directory(TDirectory *directory) {
      fHist->SetDirectory(directory);
      fHistOwner = directory;
   }
};

/// Specialized case of TH2D that displays "summary" information
/*!
 *  By "summary", we mean that each y-axis bin corresponds to a single parameter,
 *  and the x-axis corresponds to parameter values. Currently this is only supported
 *  for arrays, not individual separate parameters.
 */
class SummaryHist : public HistParam<TH2D> {
 public:
   /// Sets x-axis binning from \e hist, y-axis from \e paramArray
   SummaryHist(TH1D *hist, const DataPointer *paramArray);
   /// Empty, ~Hist<TH2D> takes care of everything
   virtual ~SummaryHist() {}
   /// Override the fill() method to act appropriately for summary histograms
   virtual Int_t fill();
};

/// Specialized case of TH1D to display scaler information
/*!
 * For scaler histograms, we put the "event" number on the x-axis and the number of
 * counts in a given event on the y-axis
 */
class ScalerHist : public HistParam<TH1D> {
 private:
   /// Counter for the scaler event number
   Int_t fEventNumber;

 public:
   /// Calls Hist<TH1D> constructor; sets fEventNumber to zero
   ScalerHist(TH1D *hist, const DataPointer *param);
   /// Empty, Hist<TH1D> destructor handles everything
   ~ScalerHist() {}
   /// Override fill() method to correspond to scaler display
   virtual Int_t fill();

 private:
   /// Extend the x-axis
   void extend(double factor);
};

// INLINE IMPLEMENTATIONS //

// HistBase

inline void rootana::HistBase::set_cut(const Cut &cut) {
   fCut.reset(cut);
}

inline bool rootana::HistBase::apply_cut() {
   /*! \returns true if fCut.get() == 0, otherwise, the value of fCut.operator() */
   return !fCut.get() ? true : fCut();
}

// HistParam

template <class T>
inline rootana::HistParam<T>::~HistParam() {
   /*!
	 * \note Delete fHist only if it is not owned by a directory; otherwise, the
	 * owning diretory must be allowed to take care of things.
	 */
   delete fHist;
   delete fParamx;
   delete fParamy;
   delete fParamz;
}

template <class T>
inline rootana::HistParam<T>::HistParam(const HistParam &other) {
   fParamx = new DataPointerT<T>(other.fParamx);
   fParamy = new DataPointerT<T>(other.fParamy);
   fParamz = new DataPointerT<T>(other.fParamz);
   fHist = new T(*(other.fHist));
   fHistOwner = fHist->GetDirectory();
}

template <class T>
inline rootana::HistParam<T> &rootana::HistParam<T>::operator=(const HistParam &other) {
   fParamx = new DataPointerT<T>(other.fParamx);
   fParamy = new DataPointerT<T>(other.fParamy);
   fParamz = new DataPointerT<T>(other.fParamz);
   fHist = new T(*(other.fHist));
   fHistOwner = fHist->GetDirectory();
   return *this;
}

/// Specialized constructor for TH1 (with parameter)
template <>
inline rootana::HistParam<TH1D>::HistParam(TH1D *hist, const DataPointer *param) : fHist(hist), fParamx(param), fParamy(DataPointer::New()), fParamz(DataPointer::New()) {
   fHistOwner = hist ? hist->GetDirectory() : 0;
}

/// Specialized constructor for TH2D (with parameters)
template <>
inline rootana::HistParam<TH2D>::HistParam(TH2D *hist, const DataPointer *paramx, const DataPointer *paramy) : fHist(hist), fParamx(paramx), fParamy(paramy), fParamz(DataPointer::New()) {
   fHistOwner = hist ? hist->GetDirectory() : 0;
   if (fHist)
      fHist->SetOption("COLZ");
}

/// Specialized constructor for TH3D (with parameters)
template <>
inline rootana::HistParam<TH3D>::HistParam(TH3D *hist, const DataPointer *paramx, const DataPointer *paramy, const DataPointer *paramz) : fHist(hist), fParamx(paramx), fParamy(paramy), fParamz(paramz) {
   fHistOwner = hist ? hist->GetDirectory() : 0;
}

/// Specialized fill() for TH1D
template <>
inline Int_t rootana::HistParam<TH1D>::fill() {
   /*! Fills the histogram if x param is valid and fCut is satisfied */
   if (exotic::utils::is_valid(fParamx->get()) && apply_cut())
      return fHist->Fill(fParamx->get());
   else
      return 0;
}

/// Specialized fill() for TH2D
template <>
inline Int_t rootana::HistParam<TH2D>::fill() {
   /*! Fills the histogram if x,y params are valid and fCut is satisfied */
   if (exotic::utils::is_valid(fParamx->get(), fParamy->get()) && apply_cut())
      return fHist->Fill(fParamx->get(), fParamy->get());
   else
      return 0;
}

/// Specialized fill() for TH3D
template <>
inline Int_t rootana::HistParam<TH3D>::fill() {
   /*! Fills the histogram if x,y,z params are valid and fCut is satisfied */
   if (exotic::utils::is_valid(fParamx->get(), fParamy->get(), fParamz->get()) && apply_cut())
      return fHist->Fill(fParamx->get(), fParamy->get(), fParamz->get());
   else
      return 0;
}

// HistFunc

template <class T>
inline rootana::HistFunc<T>::~HistFunc() {
   /*!
	 * \note Delete fHist only if it is not owned by a directory; otherwise, the
	 * owning diretory must be allowed to take care of things.
	 */
   delete fHist;
}

template <class T>
inline rootana::HistFunc<T>::HistFunc(const HistFunc &other) {
   fFuncx = other.fFuncx;
   fFuncy = other.fFuncy;
   fFuncz = other.fFuncz;
   fHist = new T(*(other.fHist));
   fHistOwner = fHist->GetDirectory();
}

template <class T>
inline rootana::HistFunc<T> &rootana::HistFunc<T>::operator=(const HistFunc &other) {
   fFuncx = other.fFuncx;
   fFuncy = other.fFuncy;
   fFuncz = other.fFuncz;
   fHist = new T(*(other.fHist));
   fHistOwner = fHist->GetDirectory();
   return *this;
}

/// Specialized constructor for TH1 (with function pointer)
template <>
inline rootana::HistFunc<TH1D>::HistFunc(TH1D *hist, const function_t func) : fHist(hist), fFuncx(func) {
   fHistOwner = hist ? hist->GetDirectory() : 0;
}

/// Specialized constructor for TH2D (with parameters)
template <>
inline rootana::HistFunc<TH2D>::HistFunc(TH2D *hist, const function_t funcx, const function_t funcy) : fHist(hist), fFuncx(funcx), fFuncy(funcy) {
   fHistOwner = hist ? hist->GetDirectory() : 0;
   if (fHist)
      fHist->SetOption("COLZ");
}

/// Specialized constructor for TH3D (with parameters)
template <>
inline rootana::HistFunc<TH3D>::HistFunc(TH3D *hist, const function_t funcx, const function_t funcy, const function_t funcz) : fHist(hist), fFuncx(funcx), fFuncy(funcy), fFuncz(funcz) {
   fHistOwner = hist ? hist->GetDirectory() : 0;
}

/// Specialized fill() for TH1D
template <>
inline Int_t rootana::HistFunc<TH1D>::fill() {
   double value = (*fFuncx)();
   if (exotic::utils::is_valid(value) && apply_cut())
      return fHist->Fill(value);
   else
      return 0;
}

// /// Specialized fill() for TH2D
template <>
inline Int_t rootana::HistFunc<TH2D>::fill() {
   double valuex = (*fFuncx)();
   double valuey = (*fFuncy)();
   /*! Fills the histogram if x,y params are valid and fCut is satisfied */
   if (exotic::utils::is_valid(valuex, valuey) && apply_cut())
      return fHist->Fill(valuex, valuey);
   else
      return 0;
}

// /// Specialized fill() for TH3D
template <>
inline Int_t rootana::HistFunc<TH3D>::fill() {
   double valuex = (*fFuncx)();
   double valuey = (*fFuncy)();
   double valuez = (*fFuncz)();
   /*! Fills the histogram if x,y,z params are valid and fCut is satisfied */
   if (exotic::utils::is_valid(valuex, valuey, valuez) && apply_cut())
      return fHist->Fill(valuex, valuey, valuez);
   else
      return 0;
}

// Scaler Hist //

inline rootana::ScalerHist::ScalerHist(TH1D *hist, const DataPointer *param) : rootana::HistParam<TH1D>(hist, param), fEventNumber(0) {
   /*!
	 * \param hist Histogram defining x-axis binning
	 * \param param DataPointer referencing the scaler parameter
	 */
   fHist->SetFillColor(30);
}

inline void rootana::ScalerHist::extend(double factor) {
   /*!
	 * Keeps bin width and minimum the same, but exends maximum
	 * by oldMax * factor
	 */
   const double Max = fHist->GetBinLowEdge(fHist->GetNbinsX() + 1);
   const double Min = fHist->GetBinLowEdge(1);
   const double binWidth = (Max - Min) / fHist->GetNbinsX();

   const double newMax = Max * factor;
   const Int_t newBins = (newMax - Min) / binWidth;

   fHist->SetBins(newBins, Min, newMax);
}

inline Int_t rootana::ScalerHist::fill() {
   /*!
	 * Puts the number of counts on the y-axis, event number on the x.
	 * If the current event number is greater than the width of the histogram,
	 * the x-axis is lengthened (by a factor of 1.5) to accomodate more events.
	 */
   if (fEventNumber > fHist->GetNbinsX())
      this->extend(1.5);

   const double counts = fParamx->get();
   fHist->SetBinContent(1 + fEventNumber++, counts);
   return counts;
}

// Summary Hist //

namespace {
inline TH2D *get_summary_2d(TH1D *xaxis, const DataPointer *param) {
   const std::string name = xaxis->GetName();
   const std::string title = xaxis->GetTitle();
   const Int_t nX = xaxis->GetNbinsX();
   const double lX = xaxis->GetXaxis()->GetBinUpEdge(0);
   const double hX = xaxis->GetXaxis()->GetBinUpEdge(nX);
   const Int_t nY = param->length();
   const double lY = 0.;
   const double hY = param->length();
   delete xaxis;
   return new TH2D(name.c_str(), title.c_str(), nX, lX, hX, nY, lY, hY);
}
} // namespace

inline rootana::SummaryHist::SummaryHist(TH1D *hist, const DataPointer *paramArray) : HistParam<TH2D>(get_summary_2d(hist, paramArray), paramArray, DataPointer::New()) {
   /*!
	 *  Calls Hist<TH2D>::Hist() with appropriate arguments for a summary histogram.
	 *  Takes name, title, and x-axis binning from \e hist, then deletes it.
	 *  Takes y-axis binning from paramArray, and sets it as the internal data pointer.
	 *  Sets the expected y-axis data poiner to DataPointerNull.
	 */
}

inline Int_t rootana::SummaryHist::fill() {
   /*! If fCut is satisfied, fills bin-by-bin whenever the corresponding param is valid */
   Int_t filled = 0;
   if (!apply_cut())
      return filled;
   for (Int_t bin = 0; bin < fHist->GetYaxis()->GetNbins(); ++bin) {
      if (exotic::utils::is_valid(fParamx->get(bin))) {
         fHist->Fill(fParamx->get(bin), bin);
         filled = 1;
      }
   }
   return filled;
}

} // namespace rootana

#endif
