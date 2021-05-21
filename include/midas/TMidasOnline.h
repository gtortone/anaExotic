#ifndef TMidasOnline_hxx_seen
#define TMidasOnline_hxx_seen
/********************************************************************\

  Name:         TMidasOnline.h
  Created by:   Konstantin Olchanski - TRIUMF

  Contents:     C++ MIDAS analyzer

  $Id$

\********************************************************************/

#include <string>
#include <vector>

#include "midas.h"

/// Request events from online shared memory data buffer

struct EventRequest
{
public:
  EventRequest* fNext;          ///< (internal use) list of all requests
  std::string   fBufferName;    ///< name of the midas data buffer, e.g. "SYSTEM"
  HNDLE         fBufferHandle;  ///< buffer handle from bm_open_buffer()
  int           fEventId;       ///< request event ID
  int           fTriggerMask;   ///< request trigger mask
  int           fSamplingType;  ///< sampling type
  int           fRequestId;     ///< request ID assigned by midas
};

/// MIDAS online connection, including access to online ODB

class TMHandlerInterface
{
 public:
  virtual ~TMHandlerInterface();
  virtual void Transition(int transition, int run_number, int transition_time) = 0;
  virtual void Event(const void* data, int data_size) = 0;
};

class TMidasOnline
{
public:

  /// User handler for run state transition events
  typedef void (*TransitionHandler)(int transition,int run_number,int trans_time);

  /// User handler for data events
  typedef void (*EventHandler)(const void*header,const void*data,int length);

public:

  std::string fHostname; ///< hostname where the mserver is running, blank if using shared memory
  std::string fExptname; ///< experiment name, blank if only one experiment defined in exptab

  HNDLE fDB; ///< ODB handle

  TransitionHandler fStartHandler;
  TransitionHandler fStopHandler;
  TransitionHandler fPauseHandler;
  TransitionHandler fResumeHandler;

  EventRequest*     fEventRequests;
  EventHandler      fEventHandler;

  std::vector<TMHandlerInterface*> fHandlers;

private:
  /// TMidasOnline is a singleton class: only one
  /// instance is allowed at any time
  static TMidasOnline* gfMidas;

  TMidasOnline(); ///< default constructor is private for singleton classes
  virtual ~TMidasOnline(); ///< destructor is private for singleton classes

public:

  /// TMidasOnline is a singleton class. Call instance() to get a reference
  /// to the one instance of this class.
  static TMidasOnline* instance();

  /// Connect to MIDAS experiment
  int connect(const char*hostname,const char*exptname,const char*progname);

  /// Disconnect from MIDAS
  int disconnect();

  /// Check for all MIDAS events (new data events, run transitions)
  bool poll(int mdelay);

  /// Sleep while checking for and answering MIDAS RPC requests (run transitions, etc)
  bool sleep(int mdelay);

  // run transitions functions

  /// Ask MIDAS to tell us about run transitions
  void registerTransitions();

  /// Specify user handlers for run transitions
  void setTransitionHandlers(TransitionHandler start,TransitionHandler stop,TransitionHandler pause,TransitionHandler resume);

  /// Check for pending transitions, call user handlers. Returns "true" if there were transitions.
  bool checkTransitions();

  // event request functions

  /// Specify user handler for data events
  void setEventHandler(EventHandler handler);

  /// Request data for delivery via callback (setEventHandler) or by polling (via receiveEvent)
  int eventRequest(const char* bufferName,int eventId,int triggerMask,int samplingType,bool poll=false);

  /// Delete data request
  void deleteEventRequest(int requestId);

  /// Receive event by polling
  int receiveEvent(int requestId, void* pevent, int size, bool async);

  /// Get buffer level (ie the number of bytes in buffer)
  int getBufferLevel();

  /// Get buffer size
  int getBufferSize();

  void RegisterHandler(TMHandlerInterface* h);
};

//end
#endif
