#
# Copyright (c) 2020-2024 Key4hep-Project.
#
# This file is part of Key4hep.
# See https://key4hep.github.io/key4hep-doc/ for further info.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
from Gaudi.Configuration import INFO
from k4FWCore import ApplicationMgr, IOSvc
from Configurables import EventDataSvc
from Configurables import DDSimpleMuonDigi

from Configurables import GeoSvc
from Configurables import UniqueIDGenSvc
from Configurables import RootHistSvc
from Configurables import Gaudi__Histograming__Sink__Root as RootHistoSink
import os

id_service = UniqueIDGenSvc("UniqueIDGenSvc")

geoservice = GeoSvc("GeoSvc")
geoservice.detectors = [os.environ["K4GEO"]+"/FCCee/CLD/compact/CLD_o2_v06/CLD_o2_v06.xml"]
geoservice.OutputLevel = INFO
geoservice.EnableGeant4Geo = False

digi = DDSimpleMuonDigi()

digi.SubDetectorName = "Vertex"
digi.KeepBarrelLayersVec = [0]
digi.KeepEndcapLayersVec = [0]
digi.muon_collections = ["ECalBarrelCollection","ECalEndcapCollection",
                         "HCalBarrelCollection","HCalEndcapCollection",
                         "HCalRingCollection","LumiCalCollection",
                         "YokeBarrelCollection","YokeEndcapCollection"]
digi.outputRelCollection = "RelationMuonHit"
digi.outputMuonCollection = "CalorimeterHit"
digi.EncodingStringParameterName = "initString"
digi.CellIDLayerString = "Layer"
digi.MuonThreshold = 0.025
digi.timethresholdMuon = 0.025
digi.calibrationCoeffmuon = 120000.0
digi.maxMuonHitEnergy = 2.0
digi.detectornameB = "YokeBarrel"
digi.detectornameE = "YokeEndcap"

#digi.ResolutionU = [0.003, 0.003, 0.003, 0.003, 0.003, 0.003]
#digi.ResolutionV = [0.003, 0.003, 0.003, 0.003, 0.003, 0.003]
#digi.SimTrackerHitCollectionName = "VertexBarrelCollection"
#digi.SimTrkHitRelCollection = "VXDTrackerHitRelations"
#digi.TrackerHitCollectionName = "VXDTrackerHits"
#digi.OutputFileName = "muon_digi_histograms.root"

iosvc = IOSvc()
iosvc.input = "/home/kkostova/Desktop/simulation/sim.edm4hep.root"
iosvc.output = "output_digi.root"

# inp.collections = [
#     "VertexBarrelCollection",
#     "EventHeader",
# ]

hps = RootHistSvc("HistogramPersistencySvc")
root_hist_svc = RootHistoSink("RootHistoSink")
root_hist_svc.FileName = "ddmuondigi_hist.root"

ApplicationMgr(TopAlg=[digi],
               EvtSel="NONE",
               EvtMax=-1,
               ExtSvc=[EventDataSvc("EventDataSvc"), root_hist_svc],
               OutputLevel=INFO,
               )