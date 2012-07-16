//-----------------------------------------------------------------------------
//
// (C) 2003     European Space Agency
//              European Space Operations Centre
//              Darmstadt, Germany
//
//-----------------------------------------------------------------------------
//
// system:      SCOSII - the satellite control and operations system -
//
// subsystem:   CPD
// 
// file:        CPDtestMain.C
//
// purpose:     test program for testing CPD subsystem
//
// comments:    
//
// history:
//
// date         author          purpose of change
// ------------+------------+---
// 2003-04-25	Haeusler G.       	Creation of the file
// 2004-05-20   S.Stellato       S2KR4.0: Use of CPD/CPA: remove use of CPDread constructor, now private. Use instance method instead
//
//----------------------------------------------------------------------------
//
extern "C" {
#include "si_proc_utilities.h"
}
//INCLUDES
#include <fstream>
#include <ctype.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

#include "STL/Set.H"

#include "IPC/IPCselect.H"
#include "TDEV/TDEVSIIpkt.H"

#include "EVE/EVEmessageHandler.H"

#include "CPD/CPDread.H"
#include "CPD/CPDutility.H"
#include "CPD/test/CPDreadFilter.H"

//////////////////////////////////////////////////////////////////////////////
// STATIC METHODS

//============================================================================
//
// NAME:          get_CPD_error_string
//
// DESCRIPTION:
//    returns string of CPDretVal
// PARAMETERS: string 1
//             string 2
// RETURNS: match flag
//----------------------------------------------------------------------------
const char *get_CPD_error_string( CPDretVal ret )
{
    static char nbuf[80];
    
    switch( ret )
    {
      case CPD_ERROR :           return "error";
      case CPD_NO_ERROR:         return "no error"; 
      case CPD_PRIMARY_HFA:      return "primary HFA";
      case CPD_BACKUP_HFA:       return "backup HFA"; 
      case CPD_NOT_LOGGED_ON:    return "not logged in"; 
      case CPD_MORE_DATA:        return "more data"; 
      case CPD_HFA_NOT_RUNNING:  return "FA not running";
      case CPD_ERROR_IN_REQUEST: return "error in request";
      case CPD_NO_RECORDS_FOUND: return "no records found";
      case CPD_END_OF_REQUEST:   return "end of request";
      case CPD_ENOUGH_DATA:      return "not enough data";
    }
    sprintf ( nbuf, "error %d", ((int)ret) );
    return nbuf;
}

//============================================================================
//
// NAME:          match
//
// DESCRIPTION:
//    match two char pointers
// PARAMETERS: string 1
//             string 2
// RETURNS: match flag
//----------------------------------------------------------------------------
static bool match (const char * s1, const char * s2)
{
    do	if (tolower(*s1++) != tolower(*s2++))
	    return false;
    while (*s1 != '\0');

    return true;
}

//============================================================================
// NAME:          usage
//
// DESCRIPTION:
//     prints usage of CPD test program
//    
// PARAMETERS: program name
// RETURNS: void
//----------------------------------------------------------------------------
static void usage (const char * progName)
{
    static	const	char	usageString []	=

	"usage: %s [ option ... ]\n"
	"list of options:\n"
		"\t-p\t<TM|TC|EV>\n"
		"\t-retrmode\n"
		"\t[-switch\tdomainId\tserver family]\n"
		"\t[-livemode]\n"
		"\t[-filter\tfilename]\n"
		"\t[-sleep\t\tmilliseconds]\n"
		"\t[-max\t\tnr. cycles]\n"
		"\t[-resume-on-error\n"
		"\t[-verbose]\n";

    fprintf(stderr, usageString, progName);
    exit(1);
}

// ### coverage ###
#ifdef GCOV_HANDLER
#include "gcov_handler.H"
#endif
//### ###
//============================================================================
// NAME:          main
//
// DESCRIPTION:
//     main of CPD test program
//    
// PARAMETERS: argc
//             argv
// RETURNS: exit code
//----------------------------------------------------------------------------
int	main (int argc, char ** argv)
{

  si_proc_init();
// ### coverage ###
#ifdef GCOV_HANDLER
sigset_t signal_set;
pthread_t sig_thread;

//block SIGUSR1 to main thread, an implicitly to all children
sigemptyset(&signal_set);
sigaddset(&signal_set,SIGUSR1);
pthread_sigmask(SIG_BLOCK,&signal_set,NULL);


MISCosServices::instance();
//create a new thread that is going to handle the SIGUSR1 signal
pthread_create(&sig_thread,NULL,gcov_sig_handler,NULL);
#endif
// ### ###

    String fileName="filter.txt";
    bool isLiveMode=false;
    bool isRetrMode=false;
    int sleepTime=0;
    CPDtype type=CPD_TM;
	int maxNrCycles=0;
	int srvFamily=0;
	int srvDomain=0;
	int verbose = 0;
	int dont_stop_on_error = 0;
     
	// read parameters
    if (argc==1)
    	usage(argv[0]);    	
    for (int i = 1; i < argc; )
    {
		char* arg = argv[i++];
		if (*arg == '-')
		{
	    	char* opt = arg + 1;
	    	if (match(opt, "filter"))
	    		fileName=argv[i++];
	    	else if (match(opt, "livemode"))
	    		isLiveMode=true;
	    	else if (match(opt, "retrmode"))
	    		isRetrMode=true;
	    	else if (match(opt, "switch"))
	    	{
	    		srvDomain=atoi(argv[i++]);
	    		srvFamily=atoi(argv[i++]);
	    	}
	    	else if (match(opt, "sleep"))
	    		sleepTime=atoi(argv[i++]);
	    	else if (match(opt, "max"))
	    		maxNrCycles=atoi(argv[i++]);
	    	else if (match(opt, "p"))
	    	{
	    		String typeStr=argv[i++];
	    		if (typeStr=="TC")
	    			type=CPD_TC;
	    		else if (typeStr=="EV")
	    			type=CPD_EV;
	    	}
	    	else if (match(opt, "verbose"))
	    		verbose++;
	    	else if (match(opt, "resume-on-error"))
	    		dont_stop_on_error = 1;
	    }
		else
	    	usage(argv[0]);    	
    }	
	
	// instanciate event handler
	EVEmessageHandler::instance("CPDtest");

	// create CPDread for telemetry 
	CPDread *cpdread=&CPDread::instance(type,"CPDtest");
			
   	CPDretVal ret=CPD_NO_ERROR;
	TDEVSIIpkt *pkt;
	int count=0;

   	// set delay
   	struct timespec delay;
   	delay.tv_sec = sleepTime/1000;
	delay.tv_nsec = (sleepTime%1000)*1000000;
	
	if ( verbose )
	{
    	if ( sleepTime > 0 ) cout << "Sleeptime =" << sleepTime << "ms" << endl;
    	if ( maxNrCycles > 0 ) cout << "Max cycles =" << maxNrCycles << endl;
    	if ( dont_stop_on_error ) cout << "Resume on errors" << endl;
	}

    // retrieval mode
    if (isRetrMode==true)
    {
		// read in filter from file
    	CPDreadFilter readFilter(fileName);
		String filterStr = readFilter.getFilterString();
		CPDrequestFilter filter;
		filter.setFilter(filterStr);
		cout << "Filter: " << (const char*)filterStr << endl;
	
		// set filter
		cpdread->setRequestFilter(filter);
    	
		Vector<TDEVSIIpkt*> newPackets;

   		fd_set fds;
   		int selRet;
   		int requestHandle;
   		
		while(ret==CPD_NO_ERROR)
		{
/*        	
			// send request
			ret=cpdread->sendRequest();
			requestHandle=cpdread->getDataResponseFd();
   		timeval   timeout = {10, 0};
    		FD_ZERO(&fds);
    		FD_SET(requestHandle, &fds);

    		// Waiting for Packet to arrive
    		selRet = IPCselect(FD_SETSIZE, &fds, NULL, NULL, &timeout);
    		if (selRet <= 0)
    		{
    		  continue;
    		}
    		// data Received
    		if (FD_ISSET(requestHandle, &fds))
    		{
			
				// getRetrievedPackets for one request
				cpdread->getRetrievedPackets(newPackets);
				// dump Data
    			for (Vector<TDEVSIIpkt*>::iterator iter = newPackets.begin();
        			iter != newPackets.end();
        			iter++)
    			{
					TDEVSIIpkt *pkt =*iter;
    				delete pkt;
    			}
        		newPackets.clear();   
        	}*/
        	
			// send request again for next packets
			ret=cpdread->sendRequest();
			if (ret!=CPD_NO_ERROR)
				break;
   			while(1)
			{
    			// Waiting for Packet to arrive
				requestHandle=cpdread->getDataResponseFd();
   		        timeval   timeout = {10, 0};
    			FD_ZERO(&fds);
    			if (requestHandle>0)
    			{
    				FD_SET(requestHandle, &fds);
    			}
    			selRet = IPCselect(FD_SETSIZE, &fds, NULL, NULL, &timeout);
    			if (selRet <= 0)
    			{
    		  		continue;
    			}
				requestHandle=cpdread->getDataResponseFd();
    			// data Received
    			if (requestHandle>0 && FD_ISSET(requestHandle, &fds))
    			{
					// get next packet
					pkt=cpdread->getRetrievedPacket(&ret);
					if (pkt)
					{
					if( dynamic_cast<TDEVSIItlmPkt*>(pkt) )
                                          	cout << "Packet " << count << "= DS:" << pkt->getStreamId() << ", SPID:" << dynamic_cast<TDEVSIItlmPkt*>(pkt)->getApId() << ", count:" << pkt->getSeqCounter() << endl;
					else
						cout << "Packet " << count << "= DS:" << pkt->getStreamId() << ", SPID:" << pkt->pktId() << ", count:" << pkt->getSeqCounter() << endl;
						delete pkt;
					}
					else
					{
						if (ret==CPD_END_OF_REQUEST)
							ret=CPD_NO_ERROR;
						break;
					}
				}
				count++;
			if (maxNrCycles>0 && count>=maxNrCycles)
			{
				ret=CPD_ERROR;
				break;    
			}
			}
    		if (sleepTime>0)
    		{
				nanosleep( &delay, NULL );
			}
		} 
	}
	else if (isLiveMode==true)
	{
		// live mode
		// read in filter from file
    	CPDreadFilter readFilter(fileName);
		String filterStr=readFilter.getFilterString();
		
		CPDliveFilter filter;
		filter.setFilter(filterStr);
				
		// get domains
		Vector<CPDdomainDS>* domainList=filter.getDomains();
		cpdread->setLiveDomains(domainList);
		delete domainList;
		// set realtime filter
		cpdread->setRealTimeFilter(filter);
		ret=cpdread->logOn();
		
   		fd_set fds;
   		int selRet;
   		int liveHandle;
   		unsigned int filter_count=0;

		while(ret==CPD_NO_ERROR)
		{
    		// Waiting for Packet to arrive
    		liveHandle=cpdread->getRealTimeDataFd();
   			timeval   timeout = {10, 0};
    		FD_ZERO(&fds);

   			if (sleepTime>0)
   			{
				nanosleep( &delay, NULL);
			}

		    // set realtime filter
		    if ( readFilter.getNumFilter() > 1 )
		    {
		        filter_count++;
		        filterStr = readFilter.getFilterString( filter_count % readFilter.getNumFilter() );
		        filter.setFilter( filterStr );
		        cpdread->setRealTimeFilter(filter);
		        if ( verbose ) cout << "Filter " << filter_count <<"= '" << filterStr << "'" << endl;
		    }
		    
    		FD_SET(liveHandle, &fds);
    		selRet = IPCselect(FD_SETSIZE, &fds, NULL, NULL, &timeout);
    		if (selRet <= 0)
    		{
    			continue;
    		}
    		// data Received
    		if (FD_ISSET(liveHandle, &fds))
    		{
				pkt=cpdread->getRealTimePacket(&ret);
				if (pkt!=NULL)
				{
                                    si_proc_process_sfs_packet(pkt->getFilingKey(), (char *)pkt->dataAddr());
				    if ( verbose ) 
				        cout << "Packet " << count << "= DS:" << pkt->getStreamId() 
				            << ", SPID:" << pkt->getFilingKey() << ", count:" << pkt->getSeqCounter() << endl;
					delete pkt;
        			count++;
				}
               	if ( ret != CPD_NO_ERROR)
               	{
               	    cout << "Error reported on packet " << count << ": " << get_CPD_error_string(ret) << endl;
               	    if ( dont_stop_on_error ) ret = CPD_NO_ERROR;
               	}
				if (maxNrCycles>0)
				{

					if (count>=maxNrCycles)
					{
						CPDliveFilter filter;
						filter.setFilter("65535:1");
						cpdread->setRealTimeFilter(filter);
                        sleep(5);
						break;    
				 	}
				}
			}
		}
		
		ret=cpdread->logOff();
	}	
	else
	{
		// get server family
		int srvFam=cpdread->getUsedServerFamily(srvDomain);
		cout << "Get actual server family for domain " << srvDomain << ": " << srvFam << endl;	
		cpdread->switchServer(srvFamily, srvDomain);
		srvFam=cpdread->getUsedServerFamily(srvDomain);
		cout << "New server family for domain " << srvDomain << ": " << srvFam << endl;	
	}	
	delete cpdread;
	cpdread=NULL;
  si_proc_cleanup();
    return 0;
}

