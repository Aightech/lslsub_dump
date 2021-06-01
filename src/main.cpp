/**
 * \file main.cpp
 * \brief TODO.
 * \author Alexis Devillard
 * \version 1.0
 * \date 08 may 2019
 */

#include <iostream> //cout,cin
#include <string> 
#include <lsl_cpp.h> //lsl library
#include <thread> // std::thread
#include <fstream> // open file
#include <sstream> //stream in file
#include <vector>
#include <ctime>
#include <string.h>

#include "tools.h" // args, usage, error


/**
 * @brief scanstream Search for the lsl streams and add them to the lslinfo vector..
 * @param to Lslinfo vector.
 * @param verbose To activate the display of the parameters of each streams.
 */
void scanStream(std::vector<lsl::stream_info>& to, bool verbose=true)
{
  std::vector<lsl::stream_info> strm_info = lsl::resolve_streams();
  if(strm_info.size()>0)
    {
      std::string channel_format_str[9] { "none",
            "cf_float32",
            "cf_double64",
            "cf_string",
            "cf_int32",
            "cf_int16",
            "cf_int8",
            "cf_int64",
            "cf_undefined"     // Can not be transmitted.
           };
      
      std::cerr << "[INFO] Available Streams:" << std::endl;
      for (int i = 0; i<strm_info.size(); i++)
	{
	  std::cerr <<"-  " <<  strm_info[i].name()  << " [ "<< strm_info[i].uid() << " ]" << std::endl;
	  if(verbose)
	    {
	      
	      std::cerr <<"   > Type:         " <<  strm_info[i].type() << std::endl;
	      std::cerr <<"   > Nb. Channels: " <<  strm_info[i].channel_count() << std::endl;
	      std::cerr <<"   > Format:       " <<  channel_format_str[strm_info[i].channel_format()] << std::endl;
	      std::cerr <<"   > Host name:    " <<  strm_info[i].hostname() << std::endl;	  
	      std::cerr <<"   > Rate:         " <<  strm_info[i].nominal_srate() << std::endl;
	      //channel_format_str[m_results[0].channel_format()];
	    }

	  //search for the stream in the already scanned ones
	  int new_strm = true;
	  for(int j =0; j < to.size(); j++)
	    {
	      if(strm_info[i].uid().compare( to[j].uid() ) == 0 )
		new_strm = false;
	    }
	  if(new_strm)
	    to.push_back(strm_info[i]);
	}
    }


}


/**
 * @brief store_stream Connect to the database and start storing the incoming stream..
 * @param strm_info the info of stream to record.
 * @param rec_on Boolean that stop the main loop of recording.
 */				// 
void store_stream(lsl::stream_info strm_info, bool *rec_on, int n)
{
  
  lsl::stream_inlet inlet(strm_info, 360, 0, false);
  std::string spacer ="";
  for(int i =0;i<n;i++)
    spacer+="\t\t\t\t\t";

  try {

    // and retrieve the chunks (note: this can of course also be done with pure std::vectors
    // instead of stereo_samples)
    std::vector<std::vector<float>> chunk;
    std::vector<double> timestamps;

    unsigned long int t=0;
    clock_t begin;
    clock_t end;
    double elapsed_secs;
    double rate;
    
    std::cerr << "[" << strm_info.name() << "] Started." << std::endl;
    while (*rec_on)
      {
	chunk.clear();
	timestamps.clear();
	if (inlet.pull_chunk(chunk, timestamps))
	  {
	    begin = clock();
	    //copy_data_db(C, strm_info.name(), chunk, timestamps, uid, &t);
	    
	    for(int i = 0; i<timestamps.size(); i++, t++)
	      {
		std::cout << timestamps[i];
		for(int j =0; j<chunk[i].size(); j++)
		  std::cout << ";"<< chunk[i][j];
		std::cout << '\n';
	      }
	    end = clock();
	    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC*1000000;
	    if(t%100==0)
	      std::cerr <<spacer <<"t:"<< elapsed_secs <<"   \xd" << std::flush;
	  }
    }
    
    std::cerr << "[" << strm_info.name() <<"] Disconnecting from lsldb...\xd" << std::flush;
    std::cerr << "[" << strm_info.name() <<"] Disconnected from lsldb." <<  std::endl;
    std::cerr << "[" << strm_info.name() <<"] Nb sample: " << t << std::endl;
  }
  catch (std::exception& e)
    {
      std::cerr << "[EXEPTION] Got an exception: " << e.what() << std::endl;
    }
  
	
  
}





int main(int argc, char* argv[])
{
 std::vector<std::string> opt_flag(
				    {"-id"});
  std::vector<std::string> opt_label(
				     {"ID of the LSL stream"});
  std::vector<std::string> opt_value(
				     {"OTB"});
  
  get_arg(argc, argv, opt_flag, opt_label, opt_value);

  //get the streams to record.
  std::vector<std::string> streams_to_get;
  streams_to_get.push_back( opt_value[0]);
  
  //scan the available streams.
  std::vector<lsl::stream_info> strm_info;
  scanStream(strm_info, false);

  //start to record the wanted streams that were available.
  std::vector<std::thread> strm_thread;
  bool rec_on = false;
  for(int i =0; i < strm_info.size(); i++)
      for(int j =0; j < streams_to_get.size(); j++)
  	  if(strm_info[i].name().compare( streams_to_get[j] ) == 0 )
	    {//launch a thread
	      rec_on = true;
	      strm_thread.push_back(std::thread(store_stream, strm_info[i], &rec_on, i));
	      std::cerr << "[INFO] Start to record: " <<  streams_to_get[j] << std::endl;
	    }

  //if at least one stream recording thread was launch 
  if(rec_on)
    {//wait for the user to stop the recording.
      std::cerr << "[INFO] Press any key then <ENTER> to stop recording and quit."<< std::endl;
      int a;
      std::cin >> a;
      //stop the main loop of recording of each thread.
      rec_on=false;
      for (auto& th : strm_thread) th.join();
    }
  else
    std::cerr << "[INFO] None of the listed streams has been found."<< std::endl;

  return 0;
  
  
  
}
