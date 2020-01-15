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

  if ( argc != 7 )
    {
      std::cout << "Usage: salvage_reader <4pt file to salvage> <4pt byte count> <4pt number of keys> <2pt file to salvage> <2pt byte count> <2pt number of keys>" << std::endl;
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

  
  std::string fSalvage2pt;
  std::string fSalvage4pt;
  std::stringstream ss;
  ss << argv[1];
  ss >> fSalvage4pt;
  ss.clear(); ss.str(std::string());
  ss << argv[4];
  ss >> fSalvage2pt;
  ss.clear(); ss.str(std::string());


  /*
    4PT MOD FILE RESURRECTION SCOPE
  */
  {
    int mover = 0;
    int byteCount = std::atoi(argv[2]);
    QDPIO::cout << "You provided a byte count of 4pt file = " << byteCount << std::endl;


    // Make a binary reader
    BinaryFileReader binRdr(fSalvage4pt);
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
    int tot4ptKeys;
    ss << argv[3];
    ss >> tot4ptKeys;
    QDPIO::cout << "You've specified that " << tot4ptKeys <<" keys are present in 4pt file" << std::endl;

    // mover = byteCount - 1 - 4 -tot4ptKeys*159; // good files
    mover = byteCount - 1 - 4 - 16 - tot4ptKeys*159; // bad files
    binRdr.seek(mover);
    pos = binRdr.currentPosition();
    QDPIO::cout << "CURRENT POSITION AND SUPPOSED LOCATION OF FIRST KEY/ADDRESS = " << pos << std::endl;
    QDPIO::cout << "BEGINNING READ OF FIRST KEY/FILE POSITION PAIR" << std::endl;


    // Make multi1d/vectors to hold key/address pairs
    multi1d<KeyHadron3PtCorr_t> keyVector(tot4ptKeys);
    std::vector<uint64_t> keyAddress(tot4ptKeys);
    // Loop to access keys
    for ( int k = 0; k < tot4ptKeys; ++k )
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
    multi1d<LatticeComplexD> tioValues; tioValues.resize(tot4ptKeys);
    // multi1d<TimeSliceIO<LatticeComplexD> > tioValues; tioValues.resize(tot4ptKeys);
    for ( int k = 0; k < tot4ptKeys; ++k )
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
    for ( int k = 0; k < tot4ptKeys; ++k )
      {
	QDPIO::cout << "Attempting to insert key/value pair #" << k << std::endl;
	// map_4.insert(keyVector[k], tioValues[k]);
	map_4.insert(keyVector[k], TimeSliceIO<LatticeComplexD>(tioValues[k],keyVector[k].snk_lorentz[7]));
	QDPIO::cout << "Inserted key/value pair #" << k << std::endl;
      }

    QDPIO::cout << "Inserted all the key/value pairs into the map" << std::endl;

    map_4.close();
  
  } // END OF 4PT RESURRECTION SCOPE


  
  /*
    2PT MOD FILE RESURRECTION SCOPE
  */
  {
    int mover = 0;
    int byteCount = std::atoi(argv[5]);
    QDPIO::cout << "You provided a byte count of 2pt file = " << byteCount << std::endl;

    int tot2ptKeys;
    ss.clear(); ss.str(std::string());
    ss << argv[6];
    ss >> tot2ptKeys;
    QDPIO::cout << "You've specified that " << tot2ptKeys << " keys are present in 2pt file" << std::endl;

    // Make a binary reader
    BinaryFileReader binRdr(fSalvage2pt);
    BinaryFileReader::pos_type pos = binRdr.currentPosition();
    


    // mover = byteCount - 1 - 4 - tot2ptKeys*127; // good files
    mover = byteCount - 1 - 4 - 16 - tot2ptKeys*127; // bad files
    binRdr.seek(mover);
    pos = binRdr.currentPosition();
    QDPIO::cout << "CURRENT POSITION AND SUPPOSED LOCATION OF FIRST 2PT KEY/ADDRESS = " << pos << std::endl;
    QDPIO::cout << "BEGINNING READ OF FIRST KEY/FILE POSITION PAIR" << std::endl;

    
    // Make multi1d/vectors to hold key/address pairs
    multi1d<KeyHadron3PtCorr_t> keyVector(tot2ptKeys);
    std::vector<uint64_t> keyAddress(tot2ptKeys);
    // Loop to access keys
    for ( int k = 0; k < tot2ptKeys; ++k )
      {
	keyVector[k].snk_lorentz.resize(0);
	QDPIO::cout << "Reading 2pt key at position = " << binRdr.currentPosition() << std::endl;
	read(binRdr, keyVector[k]);
	// Replace src name?
	keyVector[k].src_name = "pion-g5";
	QDPIO::cout << "Dummy address read " << k << std::endl;

	uint64_t dumdum;
	read(binRdr, dumdum);
	QDPIO::cout << "Reading address for 2pt key " << k << std::endl;
	read(binRdr, keyAddress[k]);

	// 2pt keys should have empty src/snk_lorentz multi1d's
	keyVector[k].snk_lorentz.resize(0);
	salvageKeyDump(keyVector[k]);
      }

    QDPIO::cout << "Read all 2pt key/position pairs" << std::endl;
    
    
    /*
      TRY TO FETCH THE 2PT VALUES AT ADDRESS
    */
    // multi1d<ComplexD> vals; vals.resize(tot2ptKeys);
    std::vector<ComplexD> vals;

    for ( int k = 0; k < tot2ptKeys; ++k )
      {
	int valLocation = keyAddress[k];
	// Stream to the location of the kth value
	binRdr.seek(valLocation);

	/*
	  CAN'T SEEM TO READ A COMPLEXD DIRECTLY FROM BINARYREADER
	*/
	double dumR, dumI;
	binRdr.read(dumR); binRdr.read(dumI);
	QDPIO::cout << "Real = " << dumR << "    Im = " << dumI << std::endl;

	vals.push_back(cmplx(Real(dumR), Real(dumI)));

      }

    QDPIO::cout << "Accessed all the 2pt values....." << std::endl;

    /*
      CREATE AN OUTPUT 2PT MOD FILE AND ATTEMPT TO REWRITE WITH CORRECT METADATA
    */
    // Name the new map
    std::string two_pt_name = "pion_PDF_2pt_remade.mod";
    // Set the metadata
    std::string meta_data2;
    {
      XMLBufferWriter file_xml;
      
      push(file_xml, "DBMetaData");
      write(file_xml, "id", std::string("pionPDF_2pt"));
      write(file_xml, "lattSize", QDP::Layout::lattSize());
      pop(file_xml);

      meta_data2 = file_xml.str();
    }

    // Open the map
    MapObjectDisk<KeyHadron3PtCorr_t, ComplexD> map_2;
    map_2.setDebug(10);
    QDPIO::cout << "Opening the 2pt database" << std::endl;

    try
      {
	map_2.insertUserdata(meta_data2);
	map_2.open(two_pt_name, std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
      }
    catch (const std::string& e) {
      QDPIO::cerr << "Trouble opening the 2pt map..." << e << std::endl;
      QDP_abort(666);
    }

    QDPIO::cout << "Successfully opened the 2pt map database...." << std::endl;
    
    // Loop over the 2pt key/value pairs and write to file
    for ( int k = 0; k < tot2ptKeys; ++k )
      {
	QDPIO::cout << "Attempting to insert 2pt key/value pair #" << k << std::endl;
	map_2.insert(keyVector[k], vals[k]);
	QDPIO::cout << "Inserted 2pt key/value pair #" << k << std::endl;
      }

    QDPIO::cout << "Inserted all the 2pt key/value pairs into the 2pt map" << std::endl;

    map_2.close();
  } // END OF 2PT RESURRECTION SCOPE



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
