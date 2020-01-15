/*
  This routine will.....
  ==> read in LatticeComplexD's (4pt data)
  ==> peekSite() to select a desired current-current separation
  ==> write out 4pt data in .sdb format in multi1d<ComplexD> containers,
  ==> where ComplexD is 4pt data for a range of src/sink seps. T
*/

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
  // QDPIO::cout << "Vector of keys filled..." << std::endl;

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
//    read(xml, path + "/Mod/Cfg", msp.modParams.cfg);
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

int main(int argc, char *argv[])
{
  // Put the machine into a known state
  QDP_initialize(&argc, &argv);

  XMLReader params_xml(argv[1]);
  QDPIO::cout<<"Reading from inixml = " << std::string(argv[1]) << std::endl;
  extKeys getKEYS; // Instantiate a structure to hold all desired keys to extract

  // Do an initial global read to set lattice dimensions and number of keys to extract
  readKeyPairs(params_xml,"/Modsdb",getKEYS);
  // Insert the lattice size into the Layout
  // There can be additional calls here like setting the number of processors to use in an SMP
  // implementation
  Layout::setLattSize(getKEYS.nrow);
  // Create the lattice layout
  // Afterwards, QDP is useable
  Layout::create();

  // // **** IF THINGS SEEM INCORRECT, RUN SOME CHECKS ****
  // xmlChecks(param);
  // QDP_abort(10);
  // // **** END OF VERBOSE CHECKS ****

  // Let's time the duration of the program
  StopWatch snoopy;
  snoopy.reset();
  snoopy.start();

  // Parameters for defining the src/snk separation
  // Set lattice extent to variables
  int Nx = Layout::lattSize()[0];
  int Ny = Layout::lattSize()[1];
  int Nz = Layout::lattSize()[2];
  if ( Nx % 2 != 0 ) Nx -= 1;
  if ( Ny % 2 != 0 ) Ny -= 1;
  if ( Nz % 2 != 0 ) Nz -= 1;
  int Nt= Layout::lattSize()[t_dir];

  // Make an sdb id name
  std::string sdb_id; // this holds the type of key structure to be filled


  // QDPIO::cout<<getKEYS.params[0].num_t<<std::endl;
  // QDPIO::cout<<getKEYS.params[1].num_t<<std::endl;
  // QDPIO::cout<<getKEYS.params[0].modParams.npt<<std::endl;
  // QDPIO::cout<<getKEYS.params[1].modParams.npt<<std::endl;

  for ( int k = 0; k < getKEYS.params.size(); k++ )
    {
      // XMLReader ele_reader(argv[1]);
      // read(ele_reader,"/Modsdb/Param/elem",getKEYS.params[k]);


      // Check that the user has supplied either a 2pt mod file or a 4pt mod file
      if ( getKEYS.params[k].modParams.npt == "2mod" )
	{
	  sdb_id = "hadron3Pt"; // needed to be compatible with ADAT
	  // This id is used by dbkeys/dbutil to anticipate written key structure
	  // n.b. Hadron3PtCorr_t structure was used for 2pt calc.
	}
      else if ( getKEYS.params[k].modParams.npt == "4mod" )
	{
	  sdb_id = "hadron3Pt"; // needed to be compatible with ADAT
	  // This is is used by dbkeys/dbutil to anticipate written key structure
	  // n.b. Hadron3PtCorr_t structure was used for 4pt calc.
	}
      else {
	QDPIO::cout << "You must specify explicitly whether a 2pt/4pt mod file is being passed..." << std::endl;
	QDPIO::cout << "Please specify '2mod' or '4mod'" << std::endl;
	QDP_abort(12);
      }

      // Instantiate the maps
      MapObjectDisk<KeyHadron3PtCorr_t, ComplexD> map_2in;
      MapObjectDisk<KeyHadron3PtCorr_t, TimeSliceIO<LatticeComplexD> > map_4in;

      // Attempt to open the map
      try {
	QDPIO::cout << "Attempting to open the mod file" << std::endl;
	// Open the map
	if ( getKEYS.params[k].modParams.npt == "2mod" )
	  {
	    // map_2in.open(getKEYS.params[k].modParams.dir+"/"+getKEYS.params[k].modParams.cfg+"/"+getKEYS.params[k].modParams.name, std::ios_base::in | std::ios_base::out);
	    map_2in.open(getKEYS.params[k].modParams.dir+"/"+getKEYS.params[k].modParams.name, std::ios_base::in | std::ios_base::out);

	  }
	if ( getKEYS.params[k].modParams.npt == "4mod" )
	  {
	    // map_4in.open(getKEYS.params[k].modParams.dir+"/"+getKEYS.params[k].modParams.cfg+"/"+getKEYS.params[k].modParams.name, std::ios_base::in | std::ios_base::out);
	    map_4in.open(getKEYS.params[k].modParams.dir+"/"+getKEYS.params[k].modParams.name, std::ios_base::in | std::ios_base::out);
	  }
      }
      catch ( const std::string &e ) {
	QDPIO::cerr << "Caught exception while opening mod file... a wizard should know better " << e << std::endl;
	QDP_abort(1);
      }
      QDPIO::cout << "OK, MapObjectDisk is now open..." << std::endl;


      // Instantiate a vector to hold all key structures in input mod file
      std::vector<KeyHadron3PtCorr_t> vector_key;
      // Fill the vector_key
      if ( getKEYS.params[k].modParams.npt == "2mod" )
	{
	  grab2Keys(map_2in, vector_key);
	}
      if ( getKEYS.params[k].modParams.npt == "4mod" )
	{
	  grab4Keys(map_4in, vector_key);
	}

  
      // // Instantiate a vector to hold booleans of whether a given key has been read
      // std::vector<bool> truth(vector_key.size());
      // // Set all entries of truth vector to false
      // for ( size_t k = 0; k < vector_key.size(); k++ )
      //   {
      //     truth[k] = false;
      //   }


      // ****************************************************
      // SET SOME MULTI1D'S FOR ACCESSING & STORING 4PT DATA
      multi1d<int> h(Nd); // multi1d of ints of size Nd - for peekSite of TimeSliceIO object...
      multi1d<int> snk_lor(6); // empty multi1d to contain mass props btwn currents, currents {gamma1, gamma2} used, and spatial separation of currents {x,y,z}
      // ****************************************************


      // Define the absolute spatial coordinates of first current insertion of 4pt fn.
      // These are unchanged for a single gauge config. (so grab first one)
      int x_abs = vector_key[0].snk_lorentz[4];
      int y_abs = vector_key[0].snk_lorentz[5];
      int z_abs = vector_key[0].snk_lorentz[6];


      // Some stuff needed for writing data to an sdb file
      // Instantiate a SerialDBKey & SerialDBData
      SerialDBKey<KeyHadron3PtCorr_t> K;
      SerialDBData<multi1d<ComplexD> > V;
      // V ---> LatticeComplexD peeked for some 'h' - multi1d<ComplexD> for various src/snk seps. T (4pt)
      // V ---> ComplexD for various src/snk seps. T (2pt)


      // Instantiate the output key structure
      KeyHadron3PtCorr_t key_out;

      // Define "output_data" here so it goes out of scope after each loop iteration and is cleared
      multi1d<ComplexD> output_2data(getKEYS.params[k].t_list.size());
      multi1d<LatticeComplexD> output_4data(getKEYS.params[k].t_list.size());

      // Data written to sdb is peeked LatticeComplexD object for increasing src/snk seps.
      multi1d<ComplexD> DATA(getKEYS.params[k].num_t);


      // bool truth_flag = false; // bool for whether "truth" contains only trues...
      // // Continue work so long as some element of "truth" vector is false
      // // ...this will allow for generalization to any gammas, momenta, etc.
      // while ( !truth_flag )
      //   {

      // whileTimer.reset();
      // whileTimer.start();

      
      try {
	// grab the key and values here
	if ( getKEYS.params[k].modParams.npt == "2mod" )
	  {
	    MapObjKeyHadron3ptCorrLookup_2pt(map_2in, vector_key, getKEYS.params[k], output_2data, key_out);
	  }
	if ( getKEYS.params[k].modParams.npt == "4mod" )
	  {
	    MapObjKeyHadron3ptCorrLookup_4pt(map_4in, vector_key, getKEYS.params[k], output_4data, key_out);

	    h[3] = key_out.snk_lorentz[7]; // all current entries in output_4data have same magic_T

	    // Make minor changes to key_out structure
	    key_out.snk_lorentz.resize(0);
	  }
      }
      catch ( const std::string &e ) {
	QDPIO::cerr << "Caught exception in lookup of MapObjectDisk file contents..." << e << std::endl;
      }

      // // For writing data with QIO
      // XMLBufferWriter rec_xml;
      // QDPFileWriter(XMLBufferWriter& xml, const std::string& path, QDP_volfmt_t qdp_volfmt, QDP_serialparallel_t qdp_serpar);
      // xml = container for metadata  // path = name of the file
      // qdp_volfmt = type of file to write // qdp_serpar = serial or parallel IO
      // QDPFileWriter out(file_xml, params.out.db, params.out.volfmt, parallel_io_type, mode_type);  
      // write(rec_xml, "key", key);
      // write(out, rec_xml, c);

      /*
	Instantiate an sdb to store the data
	sdb will hold:
	=> Key   : "KeyHadron3PtCorr_t" type
	=> Value : "ComplexD" type (peeked 4pt fn. for a given src/sink separation T)
      */
      // Instatiate the 2pt database
      BinaryStoreDB< SerialDBKey<KeyHadron3PtCorr_t>, SerialDBData<multi1d<ComplexD> > > qdp_2pt_db;
      BinaryStoreDB< SerialDBKey<KeyHadron3PtCorr_t>, SerialDBData<multi1d<ComplexD> > > qdp_4pt_db;

      // Attempt to open the desired sdb
      try{

	if ( getKEYS.params[k].modParams.npt == "2mod" )
	  {
	    if ( qdp_2pt_db.fileExists(getKEYS.params[k].sdb_name) == 0 ){
	      XMLBufferWriter file_xml; // Instatiate the QDP file writer

	      push(file_xml, "DBMetaData");
	      write(file_xml, "id", sdb_id);
	      write(file_xml, "lattSize", QDP::Layout::lattSize());
	      write(file_xml, "decay_dir",t_dir);
	      proginfo(file_xml);    // Print out basic program info
	      // write(file_xml, "Params", params.param);
	      // write(file_xml, "ensemble", params.param.ensemble);
	      // write(file_xml, "Config_info", gauge_xml);
	      pop(file_xml);

	      std::string file_str(file_xml.str());
	      qdp_2pt_db.setMaxUserInfoLen(file_str.size());
	      qdp_2pt_db.open(getKEYS.params[k].sdb_name, O_RDWR | O_CREAT, 0664); // 0664 is a regular unix file mode
	      qdp_2pt_db.insertUserdata(file_str);
	    }
	    else{ // sdb already exists...
	      qdp_2pt_db.open(getKEYS.params[k].sdb_name, O_RDWR, 0664); // 0664 is a regular unix file mode
	    }
	  }

	if ( getKEYS.params[k].modParams.npt == "4mod" )
	  {
  
	    if ( qdp_4pt_db.fileExists(getKEYS.params[k].sdb_name) == 0 ){
	      XMLBufferWriter file_xml; // Instatiate the QDP file writer

	      push(file_xml, "DBMetaData");
	      write(file_xml, "id", sdb_id);
	      write(file_xml, "lattSize", QDP::Layout::lattSize());
	      write(file_xml, "decay_dir",t_dir);
	      proginfo(file_xml);    // Print out basic program info
	      // write(file_xml, "Params", params.param);
	      // write(file_xml, "ensemble", params.param.ensemble);
	      // write(file_xml, "Config_info", gauge_xml);
	      pop(file_xml);

	      std::string file_str(file_xml.str());
	      qdp_4pt_db.setMaxUserInfoLen(file_str.size());
	      qdp_4pt_db.open(getKEYS.params[k].sdb_name, O_RDWR | O_CREAT, 0664); // 0664 is a regular unix file mode
	      qdp_4pt_db.insertUserdata(file_str);
	    }
	    else{ // sdb already exists...
	      qdp_4pt_db.open(getKEYS.params[k].sdb_name, O_RDWR, 0664); // 0664 is a regular unix file mode
	    }
	  }

      } // end of try
      catch ( const std::string &e ) {
	QDPIO::cerr << "Caught exception while opening sdb file..." << e << std::endl;
	QDP_abort(144);
      }


      // // Let's time the write process
      // StopWatch spacey;
      // spacey.reset();
      // spacey.start();


      // IF WE ARE LOOKING AT A 2PT MOD FILE, WRITE MULTI1D DIRECTLY TO FILE
      if ( getKEYS.params[k].modParams.npt == "2mod" )
	{
	  key_out.dt = 666; // dt is now a dummy tag
	  // Fill the key
	  K.key() = key_out;
	  // Populate the value
	  V.data() = output_2data;
	  // Push data to file
	  qdp_2pt_db.insert(K, V);
	  
	  // spacey.stop();
	  // QDPIO::cout << "\nTime to write multi1d of 2pt data = " << spacey.getTimeInSeconds() << std::endl;

	} // END 2PT WRITE LOOP


      // IF WE ARE LOOKING AT A 4PT MOD FILE, LOOP OVER POSSIBLE CURRENT-CURRENT SEPARATIONS
      if ( getKEYS.params[k].modParams.npt == "4mod" )
	{

	  // for ( int x = -Nx/2; x <= Nx/2; x++ )
	  // 	{
	  for ( int x = getKEYS.params[k].xmin; x <= getKEYS.params[k].xmax; x++ )
	    {
	      // StopWatch peeky;
	      // peeky.reset();
	      // peeky.start();
	    
	      h[0] = ( x + x_abs + Nx ) % Nx; // fill absolute x-separation (for peeking)
	      // for ( int y = -Ny/2; y <= Ny/2; y++ )
	      //   {
	      for ( int y = getKEYS.params[k].ymin; y <= getKEYS.params[k].ymax; y++ )
		{
		  h[1] = ( y + y_abs + Ny ) % Ny; // fill absolute y-separation (for peeking)
		  // StopWatch peeky;
		  // peeky.reset();
		  // peeky.start();
		  // for ( int z = -Nz/2; z <= Nz/2; z++ )
		  // 	{
		  // #pragma omp for
		  for ( int z = getKEYS.params[k].zmin; z <= getKEYS.params[k].zmax; z++ )
		    {
		      // if ( x != 0 || y != 0 || z != 0 ) // Only peek for nonzero separations
		      // 	{
		      h[2] = ( z + z_abs + Nz ) % Nz; // fill absolute z-separation (for peeking)
		    
		      // New snk_lorentz tag will contain mass of cc-prop, currents, & spatial sep. of currents
		      snk_lor[0] = getKEYS.params[k].ccm;
		      snk_lor[1] = getKEYS.params[k].gams[0];
		      snk_lor[2] = getKEYS.params[k].gams[1];
		      snk_lor[3] = x;
		      snk_lor[4] = y;
		      snk_lor[5] = z;
		    
		      key_out.snk_lorentz = snk_lor; // fill the currently empty snk_lorentz tag
		      key_out.dt = 666; // dt is now a dummy tag
		    
		    
		      // StopWatch peeky;
		      // peeky.reset();
		      // peeky.start();
		    
		      // Fill the key
		      K.key() = key_out;
		    
		      // // Data written to sdb is peeked LatticeComplexD object for increasing src/snk seps.
		      // multi1d<ComplexD> DATA(getKEYS.params[k].num_t);
		    
		      // Peeking loop
		      for ( int T = 0; T < getKEYS.params[k].num_t; T++ )
			{
			  // h is absolute spatial lattice coordinate!
			  DATA[T] = peekSite(output_4data[T], h);
			}
		    
		    
		      // QDPIO::cout << "TIME FOR KEY SET = " << keytime.getTimeInSeconds() << " secs" << std::endl;
		      // QDPIO::cout << "Peeked [ " << h[0] << " " << h[1] << " " << h[2] << " ] = " << peekSite(output_4data[T], h) << "\n" << std::endl;
		    
		      // Populate the value
		      V.data() = DATA;
		    
		      // QDPIO::cout << "TIME FOR PEEKING = " << keytime.getTimeInSeconds() << " secs" << std::endl;
		      // Push the key/value structure to file
		    
		      // keytime.reset();
		      // keytime.start();
		    
		      // Push data to file
		      qdp_4pt_db.insert(K, V);
		    
		      // keytime.stop();
		      // QDPIO::cout << "TIME FOR INSERTING = " << keytime.getTimeInSeconds() << " secs" << std::endl;
		    
		      // peeky.stop();
		      // QDPIO::cout << "Time to peek and write to sdb file = " << peeky.getTimeInSeconds() << " secs" << std::endl;
		      // } // end non-zero separation conditional
		    
		    } // end z loop
		  // peeky.stop();
		  // QDPIO::cout << "TIME FOR Z LOOP = " << peeky.getTimeInSeconds() << " secs" << std::endl;
		
		} // end y loop
	    
	    
	    } // end x loop


	} // END 4PT WRITE LOOP...

      // spacey.stop();
      // QDPIO::cout << "Time to write key values to file = " << spacey.getTimeInSeconds() << " secs\n" << std::endl;

      // // Check if all elements of vector_key have been matched
      // truth_flag = falseCheck(truth);

      // QDPIO::cout << "Truth flag = " << truth_flag << std::endl;

      // whileTimer.stop();
      // QDPIO::cout << "Time to complete while loop = " << whileTimer.getTimeInSeconds() << " secs" << std::endl;
      // } // end while loop - relevant when looping over all key-value pairs in mod file


      QDPIO::cout << "SDB filled, closing the file..." << std::endl;
      // Close the sdb file
      if ( getKEYS.params[k].modParams.npt == "2mod" )
	{
	  qdp_2pt_db.close();
	}
      if ( getKEYS.params[k].modParams.npt == "4mod" )
	{
	  qdp_4pt_db.close();
	}
      QDPIO::cout << "Closed the sdb..." << std::endl;

    }

 
  // Possibly shutdown the machine
  QDP_finalize();
 
  snoopy.stop();
  QDPIO::cout << "MOD_TO_SDB total time = "
	      << snoopy.getTimeInSeconds()
	      << " secs" << std::endl;
  exit(0);
}
