/*
This routine will.....
       ==> generate XML output of keys present in a provided .mod file
*/
#include <sstream>
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

// using namespace QDP;
using namespace Chroma;



void write4PtKeysToFile(MapObjectDisk<KeyHadron3PtCorr_t, TimeSliceIO<LatticeComplexD> >& the_map, XMLFileWriter& xml_out)
{
  
  std::stringstream ss;

  // Instantiate a vector of KeyHadron3ptCorr_t structures
  std::vector<KeyHadron3PtCorr_t> vec;


  // Return a vector of all available keys
  the_map.keys(vec);

  QDPIO::cout << "Number of distinct 4pt keys = " << vec.size() << std::endl;
  // Loop over the vector elements
  for ( size_t i = 0; i < vec.size(); i++ )
    {

      ss << i;
      const std::string path = "Key_" + ss.str();

      // Clear the string stream
      ss.str("");
      ss.clear();
      

      push(xml_out, path);

      write(xml_out, "num_vecs", vec[i].num_vecs);
      write(xml_out, "src_name", vec[i].src_name);
      write(xml_out, "src_smear", vec[i].src_smear);
      write(xml_out, "src_lorentz", vec[i].src_lorentz);
      write(xml_out, "src_spin", vec[i].src_spin);
      write(xml_out, "snk_name", vec[i].snk_name);
      write(xml_out, "snk_smear", vec[i].snk_smear);
      write(xml_out, "snk_lorentz", vec[i].snk_lorentz);
      write(xml_out, "snk_spin", vec[i].snk_spin);
      write(xml_out, "PiPf", vec[i].pi_pf);
      write(xml_out, "gamma", vec[i].gamma);
      write(xml_out, "links", vec[i].links);
      write(xml_out, "quark", vec[i].quark);
      write(xml_out, "dt", vec[i].dt);
      write(xml_out, "mass", vec[i].mass);
      write(xml_out, "ensemble", vec[i].ensemble);

      pop(xml_out);
    } // end loop over vector elements


  // // THE BELOW IS JUST A TEST OF ACCESSING A VALUE FROM FILE...
  // KeyHadron3PtCorr_t test_key;
  // // TimeSliceIO<LatticeComplexD> temp(LatticeComplexD, int 53);


  // multi1d<int> snk_lor(8);
  // snk_lor[0] = 7;
  // snk_lor[1] = 7;
  // snk_lor[2] = 13;
  // snk_lor[3] = 11;
  // snk_lor[4] = 12;
  // snk_lor[5] = 11;
  // snk_lor[6] = 53;
  // snk_lor[7] = 32764;
  // test_key.num_vecs = 3;
  // test_key.src_name = "pion-g5";
  // test_key.src_smear = "S";
  // test_key.src_lorentz.resize(0);
  // test_key.src_spin = 0;
  // test_key.snk_name = "pion-g5";
  // test_key.snk_smear = "S";
  // test_key.snk_lorentz = snk_lor;
  // test_key.snk_spin = 0;
  // multi1d<int> momentum(3);
  // momentum[0] = 0;
  // momentum[1] = 0;
  // momentum[2] = 8;
  // test_key.pi_pf.p_i = momentum;
  // test_key.pi_pf.p_f = momentum;
  // test_key.gamma = 0;
  // test_key.links.resize(0);
  // test_key.quark = 1;
  // test_key.dt = 26;
  // test_key.mass = "m-0.2800_m-0.2800";
  // test_key.ensemble = "";

  // int t_slice = 53;
  // // TimeSliceIO<LatticeComplexD> temp(LatticeComplexD, int);
  // LatticeComplexD C;
  // TimeSliceIO<LatticeComplexD> temp(C, t_slice);
  

  // the_map.get(test_key, temp);
  // QDPIO::cout << "Grabbed value from file" << std::endl;
  // multi1d<int> H(4);
  // H[0] = 13;
  // H[1] = 13;
  // H[2] = 13;
  // H[3] = 53;

  // // N.B. peekSite() SEEMS TO ACCEPT EITHER A 3D OR 4D MULTI1D FOR THE PEEKED LOCATION!

  // QDPIO::cout << "Test value from file = " << peekSite(C, H) << std::endl;
}

void write2PtKeysToFile(MapObjectDisk<KeyHadron3PtCorr_t, ComplexD >& the_map, XMLFileWriter& xml_out)
{
  
  std::stringstream ss;

  // Instantiate a vector of KeyHadron3ptCorr_t structures
  std::vector<KeyHadron3PtCorr_t> vec;


  // Return a vector of all available keys
  the_map.keys(vec);

  QDPIO::cout << "Number of distinct 2pt keys = " << vec.size() << std::endl;
  // Loop over the vector elements
  for ( size_t i = 0; i < vec.size(); i++ )
    {

      ss << i;
      const std::string path = "Key_" + ss.str();

      // Clear the string stream
      ss.str("");
      ss.clear();


      push(xml_out, path);

      write(xml_out, "num_vecs", vec[i].num_vecs);
      write(xml_out, "src_name", vec[i].src_name);
      write(xml_out, "src_smear", vec[i].src_smear);
      write(xml_out, "src_lorentz", vec[i].src_lorentz);
      write(xml_out, "src_spin", vec[i].src_spin);
      write(xml_out, "snk_name", vec[i].snk_name);
      write(xml_out, "snk_smear", vec[i].snk_smear);
      write(xml_out, "snk_lorentz", vec[i].snk_lorentz);
      write(xml_out, "snk_spin", vec[i].snk_spin);
      write(xml_out, "PiPf", vec[i].pi_pf);
      write(xml_out, "gamma", vec[i].gamma);
      write(xml_out, "links", vec[i].links);
      write(xml_out, "quark", vec[i].quark);
      write(xml_out, "dt", vec[i].dt);
      write(xml_out, "mass", vec[i].mass);
      write(xml_out, "ensemble", vec[i].ensemble);

      pop(xml_out);
    } // end loop over vector elements
}


// The main routine....
int main(int argc, char *argv[])
{

  if ( argc != 5 )
    {
      QDPIO::cout << "Usage: $0 <2pt mod> <4pt mod> <Nx> <Nt>" << std::endl;
      QDP_abort(6);
    }

  // Put the machine into a known state
  QDP_initialize(&argc, &argv);

  // Grab the file names
  std::string four_pt_name, two_pt_name;
  // Dimension variables
  int NX, NT;

  std::stringstream ss;
  ss << argv[1];
  ss >> two_pt_name;
  ss.clear();
  ss.str("");
  ss << argv[2];
  ss >> four_pt_name;
  ss.clear();
  ss.str("");
  ss << argv[3];
  ss >> NX;
  ss.clear(); ss.str("");
  ss << argv[4];
  ss >> NT;

  // Setup the lattice size
  // NOTE: in general, the user will need/want to set the lattice size at run-time
  multi1d<int> nrow(Nd);
  for ( int i = 0; i < Nd-1; ++i)
    {
      nrow[i] = NX; // Set spatial lattice size to NX^3
    }
  // Set the lattice temporal extent to NT
  nrow[Nd-1] = NT;
  // Lattice size is now set at NX^3 x NT

  // Insert the lattice size into the Layout
  // There can be additional calls here like setting the number of processors to use in an SMP
  // implementation
  Layout::setLattSize(nrow);

  // Create the lattice layout
  // Afterwards, QDP is useable
  Layout::create();


  // // Check that the user has supplied a 2pt mod file and a 4pt mod file
  // if ( argc != 3 )
  //   {
  //     QDPIO::cout << "Invalid number of filenames provided...\n" << std::endl;
  //     QDPIO::cout << "Usage: ./which_keys_in_mod  <2pt mod file(abs. path)>  <4pt mod file(abs. path)>\n" << std::endl;
  //     QDP_abort(1);
  //   }
  
  
  // Instantiate a MapObjectDisk (mod) object
  MapObjectDisk<KeyHadron3PtCorr_t, TimeSliceIO<LatticeComplexD> > map4_in;
  MapObjectDisk<KeyHadron3PtCorr_t, ComplexD > map2_in;


  // Instantiate the input key
  KeyHadron3PtCorr_t key_in;

  // Instantiate an output XML file
  XMLFileWriter xml_OUT_4("Available_Keys_4.xml");
  XMLFileWriter xml_OUT_2("Available_Keys_2.xml");

  // two_pt_name = "/cache/isoClover/b6p1/cl21_32_96_b6p1_m0p2800_m0p2450/pion_pdf/pdf_m-0.2800_db/0/1022/pion_PDF_2pt.mod";
  // four_pt_name = "/cache/isoClover/b6p1/cl21_32_96_b6p1_m0p2800_m0p2450/pion_pdf/pdf_m-0.2800_db/0/1022/pion_PDF_4pt.mod";


  // two_pt_name = argv[1];
  // four_pt_name = argv[2];

  // Attempt to open the 4pt map
  try {
    // Open the map
    // map4_in.open(four_pt_name, std::ios_base::in | std::ios_base::out);
    map4_in.open(four_pt_name, std::ios_base::in );
  }
  catch ( const std::string &e ) {
    QDPIO::cerr << "Caught exception while opening mod file... a wizard should know better " << e << std::endl;
    QDP_abort(1);
  }
  QDPIO::cout << "OK, 4pt MapObjectDisk is now open..." << std::endl;
  

  // //****************************************************************************************************************
  // //****************************************************************************************************************
  // // THE FOLLOWING IS JUST A TEST TO CHECK THAT NON-ZERO VALUES ARE ASSOCIATED WITH SOME KEY IN THE OPENED MOD FILES
  // KeyHadron3PtCorr_t TEST;
  // std::ostringstream os;
  
  // Real mass = -0.2800;

  // os.str("");
  // os.precision(4);
  // os <<std::fixed ;
  // os <<"m"<<mass<<"_";
  // os <<"m"<<mass;
  // TEST.mass = os.str();
  // TEST.ensemble    = "";
  // TEST.num_vecs = Nc;  /*!< Number of color std::vectors */

  // // JK NOTES: technically there could be no smearing and the following line should check if sink smearing is doing trivial point or actual shell smearing.
  // TEST.src_smear    = "S";

  // // JK NOTES: This is assuming pions
  // TEST.src_spin  = 0;
  // TEST.src_lorentz.resize(0);

  // // JK NOTES:  The way it's written the snk and src smearing are always the same.
  // TEST.snk_smear   = TEST.src_smear;

  // // JK NOTES: This is assuming pions
  // TEST.snk_spin  = 0;

  // //TEST.dt should be set later
  // TEST.dt = 0; //(t_snk - Tsrc + Nt )%Nt ;
  // TEST.links.resize(0);

  // TEST.quark = 1;       /*!< Some number indicating which quark line */
  // os.str("");

  // std::string meson_names = "pion-g5";
  // TEST.src_name = meson_names;
  // TEST.snk_name = meson_names;

  // multi1d<int> mom_in(3);

  // // Loop over momentum insertions (z-in)
  // for(int pz =-8 ; pz < 9 ; pz++)
  //   {
  //     mom_in[0] = mom_in[1] = mom_in[2] = 0;
  //     mom_in[2]=pz;
  //     TEST.pi_pf.p_i = mom_in;
  //     TEST.pi_pf.p_f = mom_in;
  //     TEST.gamma =0;

  //     TEST.dt = 10; // pion src/snk separation

  //     multi1d<int> empt;
  //     TEST.snk_lorentz = empt;

  //     KeyHadron3PtCorr_t TEST2 = TEST;
  //     multi1d<int> empty(8);
  //     TEST.snk_lorentz = empty;

  //     TEST.snk_lorentz[0] = 4; // gamma1
  //     TEST.snk_lorentz[1] = 4; // gamma2
  //     TEST.snk_lorentz[2] = 5; // current-pion src separation
  //     TEST.snk_lorentz[3] = 16; // x-sep. of currents
  //     TEST.snk_lorentz[4] = 7; // y-sep. of currents
  //     TEST.snk_lorentz[5] = 1; // z-sep of currents
  //     TEST.snk_lorentz[6] = 41; // time slice of current insertions
  


  //     // Look for valid key variables
  //     // If a valid key is given, MapObjectDisk::get(key, value) will return 0
  //     for ( int i = 41; i < 42; i++ )
  // 	{
  // 	  LatticeComplexD something;
  // 	  TimeSliceIO<LatticeComplexD> VAL(something, i); // construction of a TimeSliceIO<LatticeComplexD> requires full lattice object and time slice considered
  // 	  ComplexD VAL2;

  // 	  if ( 0 == map2_in.get(TEST2, VAL2) )
  // 	    {
  // 	      QDPIO::cout << "2pt MapGet ( pz = " << pz << " , i = " << i << " ) = " << VAL2 << std::endl;
  // 	    }
  // 	  else { QDPIO::cout << " Not a 2 key! " << std::endl; }

  // 	  if( 0 == map4_in.get(TEST, VAL) )
  // 	    {
	  
  // 	      multi1d<int> POS(4);
  // 	      POS[0] = 16;
  // 	      POS[1] = 7;
  // 	      POS[2] = 1;
  // 	      POS[3] = 41;
  // 	      POS[2] = 5;
  // 	      QDPIO::cout << "3d PEEK ( pz = " << pz << " , i = " << i << " ) = " << peekSite(something, POS) << std::endl;
  // 	    }
  // 	  else { QDPIO::cout << " Not a 4 key! " << std::endl; }
  // 	} // end i-loop

  //   } // end loop over momentum insertions (z-in)

  // // END OF THE TEST
  // // **************************************************************************************************************
  // // **************************************************************************************************************



  // Write the available 4pt keys to an output XML
  push(xml_OUT_4, "4pt_Keys");
  write4PtKeysToFile(map4_in, xml_OUT_4);
  pop(xml_OUT_4);

  map4_in.close();
  QDPIO::cout << "4pt MapObjectDisk is now closed..." << std::endl;

  xml_OUT_4.close(); // Close the 4pt xml file


  // Attempt to open the 2pt map
  try {
    // Open the map
    // map2_in.open(two_pt_name, std::ios_base::in | std::ios_base::out);
    map2_in.open(two_pt_name, std::ios_base::in );
  }
  catch ( const std::string &e ) {
    QDPIO::cerr << "Caught exception while opening mod file... a wizard should know better " << e << std::endl;
    QDP_abort(1);
  }
  QDPIO::cout << "OK, 2pt MapObjectDisk is now open..." << std::endl;

  // Write the available 2pt keys to an output XML
  push(xml_OUT_2, "2pt_Keys");
  write2PtKeysToFile(map2_in, xml_OUT_2);
  pop(xml_OUT_2);

  map2_in.close();
  QDPIO::cout << "2pt MapObjectDisk is now closed..." << std::endl;


  // Close the 2pt xml output file
  xml_OUT_2.close();

  // Possibly shutdown the machine
  QDP_finalize();

  exit(0);
}
