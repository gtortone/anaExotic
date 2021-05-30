#!/bin/bash
# ROOT
. /opt/root/bin/thisroot.sh
# MIDAS
. /opt/midas/bin/setenv.sh
# Exotic Analyzer
/opt/anaExotic/bin/anaExotic  -histos /opt/anaExotic/conf/histos.dat -detectors /opt/anaExotic/conf/detectors.json -r9090
