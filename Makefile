#
#  Makefile
#  HPSCProject4
#
#  Created by Paul Herz on 12/1/16.
#  Copyright © 2016 Paul Herz. All rights reserved.
#

TARGETA = test_int
TARGETB = test_adapt
TARGETC = test_carbon

CXX = g++
CFLAGS = -std=c++14

SRC = src/
LIB = lib/
BIN = bin/
ROOT = $(shell pwd)/
DATA = data/
NB = notebooks/
RP = reports/

AFILES = test_int.cpp
BFILES = test_adapt.cpp
CFILES = test_carbon.cpp
CLIBFILES = Vector.cpp Matrix.cpp

NOTEBOOKABC = $(NB)proj4.ipynb
NOTEBOOKC = $(NB)carbon.ipynb
REPORTABC = $(RP)proj4.pdf
REPORTC = $(RP)carbon.pdf



################################
# All target                   #
################################

all: all_bin all_data all_reports



################################
# Application binaries         #
################################

all_bin: $(TARGETA) $(TARGETB) $(TARGETC)

$(TARGETA):
	$(CXX) $(CFLAGS) -o $(BIN)$(TARGETA) $(addprefix $(SRC), $(AFILES))

$(TARGETB):
	$(CXX) $(CFLAGS) -o $(BIN)$(TARGETB) $(addprefix $(SRC), $(BFILES))

$(TARGETC):
	$(CXX) $(CFLAGS) -o $(BIN)$(TARGETC) -I $(LIB) $(addprefix $(LIB), $(CLIBFILES)) $(addprefix $(SRC), $(CFILES))


################################
# Data files                   #
################################

all_data: data_a data_b data_c

data_a: $(TARGETA)
	cd $(BIN); ./$(TARGETA) > ../$(DATA)$(TARGETA)_out.txt

data_b: $(TARGETB)
	cd $(BIN); ./$(TARGETB) > ../$(DATA)$(TARGETB)_out.txt

data_c: $(TARGETC)
	cd $(BIN); ./$(TARGETC)



################################
# Reports                      #
################################

all_reports: $(REPORTABC) $(REPORTC)

$(REPORTABC): all_data $(NOTEBOOKABC)
	cd $(ROOT)
	jupyter nbconvert --to pdf --execute $(NOTEBOOKABC) --output-dir $(ROOT)$(RP) --PDFExporter.latex_command="['xelatex','{filename}']"

$(REPORTC): data_c $(NOTEBOOKC)
	cd $(ROOT)
	jupyter nbconvert --to pdf --execute $(NOTEBOOKC) --output-dir $(ROOT)$(RP) --PDFExporter.latex_command="['xelatex','{filename}']"

################################
# Miscellaneous                #
################################

clean:
	rm -f ./$(DATA)*.txt
	rm -f ./$(BIN)$(TARGETA)
	rm -f ./$(BIN)$(TARGETB)
	rm -f ./$(BIN)$(TARGETC)
	rm -f ./$(NB)*.nbconvert.ipynb

