# ************************
# THIS WILL BUILD A salvage_reader EXECUTABLE, LINKING AGAINST CHROMA, QDP++, & LIBXML2
# ************************

CXX=/shared/gcc-5.3.0/bin/g++
#CXX=/shared/openmpi-1.10.1/gcc-5.3.0/bin/mpicxx
# CXX=mpicxx



# CXXFLAGS=-fopenmp -DUSE_OMP -O3 -std=c++0x -march=corei7-avx -mtune=corei7-avx -mno-avx -mno-aes -Xlinker -zmuldefs -I/home/cpegerer/qcd_software/cyclops/parscalar/install/chroma/include -I/home/cpegerer/qcd_software/cyclops/parscalar/install/qdp++/include -I/home/cpegerer/qcd_software/cyclops/parscalar/install/libxml2/include/libxml2 -I/home/cpegerer/qcd_software/cyclops/parscalar/install/qmp/include -I/usr/include/libxml2

# LDFLAGS=-L/home/cpegerer/qcd_software/cyclops/parscalar/install/chroma/lib -L/home/cpegerer/qcd_software/cyclops/parscalar/install/qdp++/lib -L/home/cpegerer/qcd_software/cyclops/parscalar/install/qmp/lib -L/home/cpegerer/qcd_software/cyclops/parscalar/install/libxml2/lib

# LDLIBS=-lchroma -lqdp-lapack -ldslash -lqdp -lXPathReader -lxmlWriter -lqio -llime -lxml2 -lz -lm -lqmp -lqmp -lfiledb -lfilehash

# # -llapack -lblas


###########
# DOUBLE DANGER BUILD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
###########
# CXXFLAGS=-I/data/cpegerer/scalar_DANGER_NOCHECKSUM/install/chroma-double-scalar/include -fopenmp -DUSE_OMP -O3 -std=c++0x -march=corei7-avx -mtune=corei7-avx -mno-avx -mno-aes -I/data/cpegerer/scalar_DANGER_NOCHECKSUM/install/qdp++-double-scalar/include -I/usr/include/libxml2 -I/shared/gcc-5.3.0/lib64/include

# LDFLAGS=-L/data/cpegerer/scalar_DANGER_NOCHECKSUM/install/chroma-double-scalar/lib -L/data/cpegerer/scalar_DANGER_NOCHECKSUM/install/qdp++-double-scalar/lib -L/shared/gcc-5.3.0/lib64/lib

# LDLIBS=-lchroma -lqdp-lapack -ldslash -lqdp -lXPathReader -lxmlWriter -lqio -llime -lxml2 -lz -lm -lintrin -lfiledb -lfilehash -lgmp


# ############
# # SINGLE DANGER BUILD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# ############
# CXXFLAGS=-I/data/cpegerer/scalar_DANGER_NOCHECKSUM/install/chroma-scalar/include -fopenmp -DUSE_OMP -O3 -std=c++0x -march=corei7-avx -mtune=corei7-avx -mno-avx -mno-aes -I/data/cpegerer/scalar_DANGER_NOCHECKSUM/install/qdp++-scalar/include -I/usr/include/libxml2 -I/shared/gcc-5.3.0/lib64/include

# LDFLAGS=-L/data/cpegerer/scalar_DANGER_NOCHECKSUM/install/chroma-scalar/lib -L/data/cpegerer/scalar_DANGER_NOCHECKSUM/install/qdp++-scalar/lib -L/shared/gcc-5.3.0/lib64/lib

# LDLIBS=-lchroma -lqdp-lapack -ldslash -lqdp -lXPathReader -lxmlWriter -lqio -llime -lxml2 -lz -lm -lfiledb -lfilehash -llapack -lblas -lgmp


######
# SINGLE BUILD
######
CXXFLAGS=-I/home/cpegerer/qcd_software/cyclops/scalar/install/chroma-scalar/include -fopenmp -DUSE_OMP -O3 -std=c++0x -march=corei7-avx -mtune=corei7-avx -mno-avx -mno-aes -I/home/cpegerer/qcd_software/cyclops/scalar/install/qdp++-scalar/include -I/usr/include/libxml2 -I/shared/gcc-5.3.0/lib64/include

LDFLAGS=-L/home/cpegerer/qcd_software/cyclops/scalar/install/chroma-scalar/lib -L/home/cpegerer/qcd_software/cyclops/scalar/install/qdp++-scalar/lib -L/shared/gcc-5.3.0/lib64/lib

LDLIBS=-lchroma -lqdp-lapack -ldslash -lqdp -lXPathReader -lxmlWriter -lqio -llime -lxml2 -lz -lm -lfiledb -lfilehash -llapack -lblas -lgmp


######
# DOUBLE BUILD
######
# CXXFLAGS=-I/home/cpegerer/qcd_software/cyclops/scalar/install/chroma-double-scalar/include -fopenmp -DUSE_OMP -O3 -std=c++0x -march=corei7-avx -mtune=corei7-avx -mno-avx -mno-aes -I/home/cpegerer/qcd_software/cyclops/scalar/install/qdp++-double-scalar/include -I/usr/include/libxml2 -I/shared/gcc-5.3.0/lib64/include

# LDFLAGS=-L/home/cpegerer/qcd_software/cyclops/scalar/install/chroma-double-scalar/lib -L/home/cpegerer/qcd_software/cyclops/scalar/install/qdp++-double-scalar/lib -L/shared/gcc-5.3.0/lib64/lib

# LDLIBS=-lchroma -lqdp-lapack -ldslash -lqdp -lXPathReader -lxmlWriter -lqio -llime -lxml2 -lz -lm -lintrin -lfiledb -lfilehash -lgmp




salvage_reader: salvage_reader.o
	$(CXX) $(CXXFLAGS) -o salvage_reader salvage_reader.o $(LDFLAGS) $(LDLIBS)

salvage_reader.o: salvage_reader.cc
	$(CXX) $(CXXFLAGS) -c salvage_reader.cc

clean: 
	rm -f salvage_reader salvage_reader.o
