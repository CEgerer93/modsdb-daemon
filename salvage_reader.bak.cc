/*
  This routine will.....
  ==> read in LatticeComplexD's (4pt data)
  ==> peekSite() to select a desired current-current separation
  ==> write out 4pt data in .sdb format in multi1d<ComplexD> containers,
  ==> where ComplexD is 4pt data for a range of src/sink seps. T
*/

#include <stdlib.h>
#include <stdint.h>
#include "chroma.h"
#include "io/xml_group_reader.h"
// #include "io/enum_io/enum_qdpvolfmt_io.h"
#include "util/ferm/key_val_db.h"
#include "util/ferm/key_hadron_3pt_corr.h"
// #include "inline_pionPDF_w.h"

/* QDP MAP HEADERS */
#include "qdp_map_obj_disk.h"
#include "qdp_disk_map_slice.h"
/* THESE MAY NOT NEED TO BE INCLUDED */
#include "qdp_map_obj_memory.h"
#include "qdp_map_obj_null.h"

using namespace Chroma;


// Define some global variables
// THESE ARE POTENTIALLY BAD FOR MORE GENERAL INSTANCES....
int t_dir = 3;


// Define a struct of strings for mod file
struct mod {
  std::string dir, cfg, name, npt;
};

// Here the mod_to_sdb structure is defined
struct modsdbParams {
  int num_t, xmin, xmax, ymin, ymax, zmin, zmax;
  multi1d<int> t_list;
  // multi1d<int> nrow;
  multi1d<int> gams;
  multi1d<int> p_i;
  multi1d<int> p_f;
  int q_sq;
  int ccm;
  struct mod modParams;
  std::string sdb_name;
};

struct extKeys {
  multi1d<modsdbParams> params;
  multi1d<int> nrow;
};

void grab2Keys(MapObjectDisk<KeyHadron3PtCorr_t, ComplexD>& the_map, std::vector<KeyHadron3PtCorr_t>& vec)
{
  
  the_map.keys(vec); // return all available keys
  QDPIO::cout << "Vector of keys filled..." << std::endl;

}

void grab4Keys(MapObjectDisk<KeyHadron3PtCorr_t, TimeSliceIO<LatticeComplexD> >& the_map, std::vector<KeyHadron3PtCorr_t>& vec)
{

  the_map.keys(vec); // return all available keys
  // QDPIO::cout << "Vector of keys filled..." << std::endl;

}

void SORT(std::vector<KeyHadron3PtCorr_t>& spec)
{
  // // Let's time the duration of the sort operation
  // StopWatch snoop;
  // snoop.reset();
  // snoop.start();

  int spec_size = static_cast<int>(spec.size());


  std::vector<int> vec_dt;
  for ( int i = 0; i < spec_size; i++ )
    {
      vec_dt.push_back( spec[i].dt ); // store the dt's for sorting
      // QDPIO::cout << vec_dt[i] << std::endl;
    }

  // Sort the 'vec_dt' in ascending order
  std::sort(vec_dt.begin(), vec_dt.end());

  std::vector<KeyHadron3PtCorr_t> temp(spec_size);
  // Now sort the vector of keys "spec" based on the sorted vector "vec_dt"
  for ( int k = 0; k < spec_size; k++ )
    {
      for ( int j = 0; j < spec_size; j++ )
	{
	  if ( vec_dt[k] == spec[j].dt )
	    {
	      temp[k] = spec[j];
	      // QDPIO::cout << temp.size() << std::endl;
	    }
	}
    }

  // "temp" is now a vector of keys sorted based on increasing 'dt'
  // Set "spec" equal to "temp"
  spec = temp;

  // snoop.stop();
  // QDPIO::cout << "SORT total time = "
  // 		<< snoop.getTimeInSeconds()
  // 		<< " secs" << std::endl;
  
}

void MapObjKeyHadron3ptCorrLookup_2pt(MapObjectDisk<KeyHadron3PtCorr_t, ComplexD>& the_map, std::vector<KeyHadron3PtCorr_t>& vec, modsdbParams& p, multi1d<ComplexD>& cd_array, KeyHadron3PtCorr_t& key_OUT)
{

  // Instantiate a vector containing a subset of 'key_vector' (i.e. "vec")
  std::vector<KeyHadron3PtCorr_t> special;

  // Master mom_in, mom_out to match
  multi1d<int> P_in = p.p_i;
  multi1d<int> P_out = p.p_f;

  QDPIO::cout<<"\n******** Trying Match *********"<<std::endl;
  QDPIO::cout<<"p_i = "<<P_in[0]<<" "<<P_in[1]<<" "<<P_in[2]<<std::endl;
  QDPIO::cout<<"p_f = "<<P_out[0]<<" "<<P_out[1]<<" "<<P_out[2]<<std::endl;
  QDPIO::cout<<"*******************************"<<std::endl;

  // Loop over keys in vector of all keys "vec"
  // Add an element of "vec" to "special" if element has desired P_in/P_out
  for ( int j = 0; j < static_cast<int>(vec.size()); j++ )
    {
      
      // Only add to the 'special' vector if current master variables are matched
      if ( vec[j].pi_pf.p_i == P_in && vec[j].pi_pf.p_f == P_out )
	{
	  special.push_back(vec[j]);
	}
      else if ( j == static_cast<int>(vec.size())-1 && special.size() == 0 ) {
	QDPIO::cout<<"Key not found : check nodes"<<std::endl;
	QDP_abort(777);
      }
      
    } // loop over unmatched keys completed



  // N.B. Keys in 'special' are currently unsorted in src/snk sep. T (dt in key)
  SORT(special);
  // keys in special are now sorted based on ascending 'dt'
  key_OUT = special[0]; // we will use most of original key structure...

  ComplexD temp;
  // Loop over keys in "special" and add to cd_array
  for ( int i = 0; i < static_cast<int>(special.size()); i++ )
    {
      the_map.get(special[i], temp); // grab the value associated with the current key
      // 0 returned on successful read; temp should now be populated

      cd_array[i] = temp; // fill the multi1d of ComplexD's
      // each element is a ComplexD for a different src/snk sep.
      // array returned to main() for writing to sdb
    }
}

void MapObjKeyHadron3ptCorrLookup_4pt(MapObjectDisk<KeyHadron3PtCorr_t, TimeSliceIO<LatticeComplexD> >& the_map, std::vector<KeyHadron3PtCorr_t>& vec, modsdbParams& p, multi1d<LatticeComplexD>& lcd_array, KeyHadron3PtCorr_t& key_OUT)
{

  // Instantiate a vector containing a subset of 'key_vector' (i.e. "vec")
  std::vector<KeyHadron3PtCorr_t> special;

  // StopWatch loopy;
  // loopy.reset();
  // loopy.start();  
  
  // Master cc-prop mass, gamma1, gamma2, mom_in, mom_out to match
  int cc = p.ccm;
  int g1 = p.gams[0];
  int g2 = p.gams[1];
  multi1d<int> P_in = p.p_i;
  multi1d<int> P_out = p.p_f;
  
  QDPIO::cout<<"\n******** Trying Match *********"<<std::endl;
  QDPIO::cout<<"cc = "<<cc<<std::endl;
  QDPIO::cout<<"g1 = "<<g1<<std::endl;
  QDPIO::cout<<"g2 = "<<g2<<std::endl;
  QDPIO::cout<<"p_i = "<<P_in[0]<<" "<<P_in[1]<<" "<<P_in[2]<<std::endl;
  QDPIO::cout<<"p_f = "<<P_out[0]<<" "<<P_out[1]<<" "<<P_out[2]<<std::endl;
  QDPIO::cout<<"*******************************"<<std::endl;



  // Loop over all keys in vector of all keys "vec"
  // Add an element of "vec" to "special" if element has desired ccm/gammas/p_i/p_f
  for ( int j = 0; j < static_cast<int>(vec.size()); j++ )
    {
      
      // Only add to the 'special' vector if current master variables are matched
      if ( vec[j].snk_lorentz[0] == cc && vec[j].snk_lorentz[1] == g1 && vec[j].snk_lorentz[2] == g2 && vec[j].pi_pf.p_i == P_in && vec[j].pi_pf.p_f == P_out )
	{
	  special.push_back(vec[j]);
	}
      else if ( j == static_cast<int>(vec.size())-1 && special.size() == 0 ) {
	QDPIO::cout<<"Key not found : check nodes"<<std::endl;
	QDP_abort(777);
      }
    } // loop over unmatched keys completed

  // loopy.stop();
  // QDPIO::cout << "Time to loop through unmatched keys = " << loopy.getTimeInSeconds() << " secs" << std::endl;
  // // THIS TAKES ~10^-5 SECONDS....


  // N.B. Keys in 'special' are currently unsorted in src/snk sep. T (dt in key)
  SORT(special);
  // keys in special are now sorted based on ascending 'dt'
  key_OUT = special[0]; // we will use most of original key structure...

  int magic_T = special[0].snk_lorentz[7];
  // special[0].snk_lorentz[7] should equal time slice at which currents are inserted

  LatticeComplexD c_temp;
  TimeSliceIO<LatticeComplexD> temp(c_temp, magic_T);


  // loopy.reset();
  // loopy.start();

  // Loop over keys in special and add to lcd_array
  for ( int i = 0; i < static_cast<int>(special.size()); i++ )
    {
      // QDPIO::cout << "Getting element [ " << i << " ] from the map..." << std::endl;
      the_map.get(special[i], temp); // grab the value associated with the current key
      // 0 returned on successful read; temp should now be populated

      lcd_array[i] = temp.getObject(); // fill the multi1d of TimeSliceIO<LatticeComplexD> objects
      // each element is a LatticeComplexD (3D) for a different src/snk sep.
      // array returned to main() for writing to sdb
    }

  // loopy.stop();
  // QDPIO::cout << "Time to get matched lattice objects from map = " << loopy.getTimeInSeconds() << " secs" << std::endl;
  // // THIS TAKES ~.25 SECONDS

}

// bool falseCheck(std::vector<bool>& TRUTH)
// {
//   // StopWatch foo;
//   // foo.reset();
//   // foo.start();

//   // returns "true" if all elements of TRUTH are true

//   bool res = true;

//   for ( int i = 0; i < static_cast<int>(TRUTH.size()); i++ )
//     {
//       res = res && TRUTH[i];
      
//       // // Output elements that are false
//       // if ( !TRUTH[i] )
//       // 	{
//       // 	  QDPIO::cout << "TRUTH[ " << i << " ] " << std::endl;
//       // 	}
//     }

//   // bool res = std::all_of( TRUTH.begin(), TRUTH.end(), [](bool B) { return B; });

//   // foo.stop();
//   // QDPIO::cout << "Time to loop through truth vector " << foo.getTimeInSeconds() << " secs" << std::endl;

//   return res;

// }

void xmlChecks(modsdbParams& p) { // run if things don't seem right
  // **** If things seem incorrect, here are some checks ****
  QDPIO::cout<<"Running some verbose checks"<<std::endl;
  QDPIO::cout<<"Read value of num_t = "<< p.num_t<<std::endl;
  for ( int i=0; i < p.num_t; i++ ) {
    QDPIO::cout<<p.t_list[i]<<" ";
  }
  QDPIO::cout<<"\n";
  QDPIO::cout<<"xmin = "<<p.xmin<<std::endl;
  QDPIO::cout<<"xmax = "<<p.xmax<<std::endl;
  QDPIO::cout<<"ymin = "<<p.ymin<<std::endl;
  QDPIO::cout<<"ymax = "<<p.ymax<<std::endl;
  QDPIO::cout<<"zmin = "<<p.zmin<<std::endl;
  QDPIO::cout<<"zmax = "<<p.zmax<<std::endl;
  QDPIO::cout<<"p_i = "<<p.p_i[0]<<" "<<p.p_i[1]<<" "<<p.p_i[2]<<std::endl;
  QDPIO::cout<<"p_f = "<<p.p_f[0]<<" "<<p.p_f[1]<<" "<<p.p_f[2]<<std::endl;
  QDPIO::cout<<"q_squared = "<<p.q_sq<<std::endl;
  QDPIO::cout<<"g1 = "<<p.gams[0]<<std::endl;
  QDPIO::cout<<"g2 = "<<p.gams[1]<<std::endl;
  QDPIO::cout<<"ccm = "<<p.ccm<<std::endl;
  // for ( int i=0; i < Nd; i++ ) {
  //   QDPIO::cout<<p.nrow[i]<<" ";
  // }
  QDPIO::cout<<"\n";
  QDPIO::cout<<"mod dir = "<<p.modParams.dir<<std::endl;
  QDPIO::cout<<"mod cfg = "<<p.modParams.cfg<<std::endl;
  QDPIO::cout<<"mod name = " <<p.modParams.name<<std::endl;
  QDPIO::cout<<"mod npt = "<<p.modParams.npt<<std::endl;
  QDPIO::cout<<"sdb name = "<<p.sdb_name<<std::endl;

  QDP_abort(8);
}

// Perform the read from the ini xml
void read(XMLReader& xml,const std::string& path, modsdbParams& msp) 
{
  try {
    read(xml, path + "/Jsrc_sep/num_t", msp.num_t);
    read(xml, path + "/Jsrc_sep/times", msp.t_list);
    read(xml, path + "/Xi/xsep/xmin", msp.xmin);
    read(xml, path + "/Xi/xsep/xmax", msp.xmax);
    read(xml, path + "/Xi/ysep/ymin", msp.ymin);
    read(xml, path + "/Xi/ysep/ymax", msp.ymax);
    read(xml, path + "/Xi/zsep/zmin", msp.zmin);
    read(xml, path + "/Xi/zsep/zmax", msp.zmax);
    read(xml, path + "/PiPf/p_f", msp.p_f);
    read(xml, path + "/PiPf/p_i", msp.p_i);
    read(xml, path + "/PiPf/q_sq", msp.q_sq);
    read(xml, path + "/gams", msp.gams);
    read(xml, path + "/ccm", msp.ccm);
    // read(xml, path + "/nrow", msp.nrow);
    read(xml, path + "/Mod/Dir", msp.modParams.dir);
    read(xml, path + "/Mod/Cfg", msp.modParams.cfg);
    read(xml, path + "/Mod/Name", msp.modParams.name);
    read(xml, path + "/Mod/Npt", msp.modParams.npt);
    read(xml, path + "/sdb/name", msp.sdb_name);
  } catch ( const std::string& e ) {
    std::cerr<<"Provided nodes are incorrect or duplicated at a given tree level...\n====> "<<e<<std::endl;
    QDP_abort(666);
  }
}

void readKeyPairs(XMLReader& xml,const std::string& path, extKeys& ek)
{
  try {
    read(xml, path + "/nrow", ek.nrow);
    read(xml, path + "/Params", ek.params);
  } catch ( const std::string& e ) {
    std::cerr<<"Caught exception trying to read one of provided modsdbParams structure"<<e<<std::endl;
    QDP_abort(777);
  }
}



void salvageKeyDump(KeyHadron3PtCorr_t& sKey)
{
  QDPIO::cout << "sKey num_vecs = " << sKey.num_vecs << std::endl;
  QDPIO::cout << "sKey p_i = " << sKey.pi_pf.p_i[0] << " "
	      << sKey.pi_pf.p_i[1] << " " << sKey.pi_pf.p_i[2] << std::endl;
  QDPIO::cout << "sKey p_f = " << sKey.pi_pf.p_f[0] << " "
	      << sKey.pi_pf.p_f[1] << " " << sKey.pi_pf.p_f[2] << std::endl;
  QDPIO::cout << "sKey gamma = " << sKey.gamma << std::endl;
  QDPIO::cout << "sKey links size = " << sKey.links.size() << std::endl;
  QDPIO::cout << "sKey dt = " << sKey.dt << std::endl;
  QDPIO::cout << "sKey quark = " << sKey.quark << std::endl;
  QDPIO::cout << "sKey src_name = " << sKey.src_name << std::endl;
  QDPIO::cout << "sKey src_smear = " << sKey.src_smear << std::endl;
  QDPIO::cout << "sKey src_lorentz size = " << sKey.src_lorentz.size() << std::endl;
  QDPIO::cout << "sKey src_spin = " << sKey.src_spin << std::endl;
  QDPIO::cout << "sKey snk_name = " << sKey.snk_name << std::endl;
  QDPIO::cout << "sKey snk_smear = " << sKey.snk_smear << std::endl;
  QDPIO::cout << "sKey snk_lorentz size = " << sKey.snk_lorentz.size() << std::endl;
  QDPIO::cout << "sKey snk_lorentz[0] = " << sKey.snk_lorentz[0] << std::endl;
  QDPIO::cout << "sKey snk_lorentz[1] = " << sKey.snk_lorentz[1] << std::endl;
  QDPIO::cout << "sKey snk_lorentz[2] = " << sKey.snk_lorentz[2] << std::endl;
  QDPIO::cout << "sKey snk_lorentz[3] = " << sKey.snk_lorentz[3] << std::endl;
  QDPIO::cout << "sKey snk_lorentz[4] = " << sKey.snk_lorentz[4] << std::endl;
  QDPIO::cout << "sKey snk_lorentz[5] = " << sKey.snk_lorentz[5] << std::endl;
  QDPIO::cout << "sKey snk_lorentz[6] = " << sKey.snk_lorentz[6] << std::endl;
  QDPIO::cout << "sKey snk_lorentz[7] = " << sKey.snk_lorentz[7] << std::endl;
  QDPIO::cout << "sKey snk_spin = " << sKey.snk_spin << std::endl;
  QDPIO::cout << "sKey mass = " << sKey.mass << std::endl;
  QDPIO::cout << "sKey ensemble = " << sKey.ensemble << "\n\n" << std::endl;

  // QDP_abort(20);
}



void salvageValDump(LatticeComplexD& l, KeyHadron3PtCorr_t& k)
{
  // Set a summy spatial separation to peek
  multi1d<int> H(Nd);
  H[0]=0; H[1]=0; H[2]=1; H[3]=k.snk_lorentz[7];

  QDPIO::cout << "Testing a peek of accessed object.... = "
              << real(peekSite(l, H)) << " " << imag(peekSite(l, H)) << std::endl;
  

  // QDP_abort(30);

}



int main(int argc, char *argv[])
{

  if ( argc != 4 )
    {
      std::cout << "Usage: salvage_reader <file to salvage> <byte count> <number of keys>" << std::endl;
      exit(1);
    }

  // Put the machine into a known state
  QDP_initialize(&argc, &argv);
  // Set the layout
  multi1d<int> nrow(Nd);
  nrow[0]=32; nrow[1]=32; nrow[2]=32; nrow[3]=64;
  Layout::setLattSize(nrow);
  Layout::create();

  QDPIO::cout << "QDP initialized..." << std::endl;


  std::string fSalvage;
  std::stringstream ss;
  ss << argv[1];
  ss >> fSalvage;
  ss.clear(); ss.str(std::string());

  int mover = 0;
  int byteCount = std::atoi(argv[2]);
  QDPIO::cout << "You provided a byte count of file = " << byteCount << std::endl;


  // Make a binary reader
  BinaryFileReader binRdr(fSalvage);
  BinaryFileReader::pos_type pos = binRdr.currentPosition();


  // // Seek to the location of the final key
  // // Checksum bytes: (byteCount - 5) --> (byteCount - 1)
  // // Size of key/fileposition data: Attempting to read the final four bytes = 159
  // mover = byteCount -1 - 4 - 159; // This appears to be correct mover for good files

  // // mover = byteCount - 1 - 4 - 16 - 159; // This seems to recover last key by skipping junk at end of bad files
  // binRdr.seek(mover);
  // pos = binRdr.currentPosition();
  // QDPIO::cout << "Current position and supposed location of final key/address = " << pos << std::endl;



  /*
    5/8/19: LET'S TRY TO STREAM TO LOCATION OF WOULD BE FIRST KEY AND GRAB THAT ONE AND ALL SUBSEQUENT KEYS
  */
  int totKeys;
  ss << argv[3];
  ss >> totKeys;
  QDPIO::cout << "You've specified that " << totKeys <<" keys are present in file" << std::endl;

  // mover = byteCount - 1 - 4 -totKeys*159; // good files
  mover = byteCount - 1 - 4 - 16 - totKeys*159; // bad files
  binRdr.seek(mover);
  pos = binRdr.currentPosition();
  QDPIO::cout << "CURRENT POSITION AND SUPPOSED LOCATION OF FIRST KEY/ADDRESS = " << pos << std::endl;
  QDPIO::cout << "BEGINNING READ OF FIRST KEY/FILE POSITION PAIR" << std::endl;


  // Make multi1d/vectors to hold key/address pairs
  multi1d<KeyHadron3PtCorr_t> keyVector(totKeys);
  std::vector<uint64_t> keyAddress(totKeys);
  // Loop to access keys
  for ( int k = 0; k < totKeys; ++k )
    {
      QDPIO::cout << "Reading key at position = " << binRdr.currentPosition() << std::endl;
      read(binRdr, keyVector[k]);
      // Add back in the src name
      keyVector[k].src_name = "pion-g5";
      QDPIO::cout << "Dummy address read " << k << std::endl;
      
      uint64_t dumdum;
      read(binRdr, dumdum);
      QDPIO::cout << "Reading address for key " << k << std::endl;
      read(binRdr, keyAddress[k]);

      // salvageKeyDump(keyVector[k]);
    }

  QDPIO::cout << "Read all key/position pairs" << std::endl;
  // salvageKeyDump(keyVector[0]);


  /*
    TRY TO FETCH THE VALUES AT ADDRESS
  */
  multi1d<LatticeComplexD> tioValues; tioValues.resize(totKeys);
  // multi1d<TimeSliceIO<LatticeComplexD> > tioValues; tioValues.resize(totKeys);
  for ( int k = 0; k < totKeys; ++k )
    {
      int valLocation = keyAddress[k];
      // Stream to the location of the kth value
      binRdr.seek(valLocation);

      // Now instantiate an empty TimeSliceIO<LatticeComplexD> object
      LatticeComplexD lcd = zero;
      TimeSliceIO<LatticeComplexD> tio(lcd, keyVector[k].snk_lorentz[7]);
      // TimeSliceIO<LatticeComplexD> tio;
      read(binRdr, tio);

      // tioValues.push_back(tio);
      // tioValues.push_back(lcd);
      tioValues[k] = lcd;
      // tioValues[k] = tio;

      // salvageValDump(lcd, keyVector[k]);
    }

  QDPIO::cout << "Accessed all the values....." << std::endl;


  /*
    CREATE AN OUTPUT MOD FILE AND ATTEMPT TO REWRITE WITH CORRECT METADATA
  */
  // Name the new map
  std::string four_pt_name = "pion_PDF_4pt_remade.mod";
  // Set the metadata
  std::string meta_data4;
  {
    XMLBufferWriter file_xml;
    
    push(file_xml, "DBMetaData");
    write(file_xml, "id", std::string("pionPDF_4pt"));
    write(file_xml, "lattSize", QDP::Layout::lattSize());
    pop(file_xml);
    
    meta_data4 = file_xml.str();
  }

  // Open the map
  MapObjectDisk<KeyHadron3PtCorr_t, TimeSliceIO<LatticeComplexD> > map_4;
  map_4.setDebug(10);
  QDPIO::cout << "Opening the database" << std::endl;

  try
    {
      map_4.insertUserdata(meta_data4);
      map_4.open(four_pt_name, std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
    }
  catch (const std::string& e) {
    QDPIO::cerr << "Trouble while opening map..." << e << std::endl;
    QDP_abort(666);
  }

  QDPIO::cout << "Successfully opened the map database...." << std::endl;
  
  // Loop over the key/value pairs and write to file
  for ( int k = 0; k < totKeys; ++k )
    {
      QDPIO::cout << "Attempting to insert key/value pair #" << k << std::endl;
      // map_4.insert(keyVector[k], tioValues[k]);
      map_4.insert(keyVector[k], TimeSliceIO<LatticeComplexD>(tioValues[k],keyVector[k].snk_lorentz[7]));
      QDPIO::cout << "Inserted key/value pair #" << k << std::endl;
    }

  QDPIO::cout << "Inserted all the key/value pairs into the map" << std::endl;

  map_4.close();

  QDP_finalize();

  exit(0);
}

//   // Key/Value pairs living in the maps
//   // 2pt value is simply a ComplexD
//   // typedef QDP::KeyHadron3ptCorr_t K;
//   // typedef QDP::TimeSliceIO<LatticeComplexD> V4;


//   // QDPIO::cout << "Beginning read of the final key/file position pair" << std::endl;
//   KeyHadron3PtCorr_t lastKey;
//   read(binRdr, lastKey);
//   // int num_vecs;
//   // // read(binRdr, num_vecs);
//   // // QDPIO::cout << "GENUINE num_vecs = " << num_vecs << std::endl;
//   // read(binRdr, lastKey.num_vecs);
//   // QDPIO::cout << "Read num_vecs = " << lastKey.num_vecs << std::endl;
//   // pos = binRdr.currentPosition();
//   // QDPIO::cout << "Following key read, current position = " << pos << std::endl;
  
//   // // Keys appear to be 143 bytes long
//   // // Address is the final 16 bytes for a total of 159 bytes per key/value position
  
//   // uint64_t lastVal;
//   // QDPIO::cout << "Reading the last value's address as a 32bit unsigned int" << std::endl;
//   // QDPIO::cout << "Current location = " << binRdr.currentPosition() << std::endl;
//   // read(binRdr, lastVal);

//   // QDPIO::cout << "Found the final value's 32bit unsigned int address = " << lastVal << std::endl;
//   // int lastValAddress;

//   // with uint64_t I get an address of 13
//   // with uint32_t I get an address of 0


//   // Lets see if we can read a 4pt Key
//   // ..will need byte extent of key from test meltemi job


//   /*
//     5/8/19: TRIED INSTANTIATING STRUCTS OR UNSIGNED CHARS OF SIZE 16 TO READ ADDRESS
//     SIMILAR TO AS IS DONE IN PRIVATE MEMBERS OF QDP_MAP_OBJ_DISK.H
//     DIDN'T WORK, NOT DID I WANT TO DEFINE MORE READ/WRITE FUNCTIONS IN THE BINARY READER

//     RETURNING TO READING TWO UNSIGNED 64-BIT INTS, AND TAKING SECOND
//     HOPEFULLY THIS WORKS
//   */

//   // Maybe skip first 8 bytes of address
//   uint64_t dumdum;
//   read(binRdr, dumdum);
//   // Now read the last 8 bytes as address

//   uint64_t finalKeyAddress;
//   read(binRdr,finalKeyAddress);
//   QDPIO::cout << "Final key address = " << finalKeyAddress << std::endl;
//   binRdr.seek(finalKeyAddress);
//   pos = binRdr.currentPosition();
//   QDPIO::cout << "\n\nSitting at start of final value at location = "
// 	      << pos << std::endl;


//   // Now instantiate an empty TimeSliceIO<LatticeComplexD> object
//   LatticeComplexD lcd;
//   TimeSliceIO<LatticeComplexD> tio(lcd, lastKey.snk_lorentz[7]);
//   read(binRdr, tio);

//   // Set a summy spatial separation to peek
//   multi1d<int> H(Nd);
//   H[0]=0; H[1]=0; H[2]=1; H[3]=lastKey.snk_lorentz[7];

  
//   QDPIO::cout << "Testing a peek of accessed object.... = "
// 	      << real(peekSite(lcd, H)) << " " << imag(peekSite(lcd, H)) << std::endl;
  
//   QDPIO::cout << "Now sitting as position = " << binRdr.currentPosition() << std::endl;

//   QDP_abort(10);

  
//   QDPIO::cout << "Current position = " << pos << std::endl;
//   QDPIO::cout << "Streaming to end..." << std::endl;
//   QDPIO::cout << "Byte count = " << byteCount << std::endl;
  
//   mover = byteCount - 4;
//   binRdr.seek(mover);
//   QDPIO::cout << "After seeking, position = " << binRdr.currentPosition() << std::endl;


//   QDPIO::cout << "Attempting to read the final four bytes = " << std::endl;
//   mover = byteCount-1;
//   binRdr.seek(mover);


//   // BYTES ARE ORDERED 0-(BYTECOUNT-1)
//   // n.b. above, byteCount + 1 returns a final position of "-1" indicating out of file


//   uint32_t checksum;
//   read(binRdr, checksum);
//   QDPIO::cout << "Checksum = " << checksum << std::endl;
  

//   QDPIO::cout << "Final position = " << binRdr.currentPosition() << std::endl;
//   QDPIO::cout << "True checksum = " << binRdr.getChecksum() << std::endl;
  

//   QDP_abort(9);
