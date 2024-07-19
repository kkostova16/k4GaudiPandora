#
# Copyright (c) 2019-2024 Key4hep-Project.
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
import os

from Gaudi.Configuration import *

from Configurables import LcioEvent, k4DataSvc, MarlinProcessorWrapper
from k4MarlinWrapper.parseConstants import *
algList = []


CONSTANTS = {
             'CalorimeterIntegrationTimeWindow': "10",
}

parseConstants(CONSTANTS)


# For converters
from Configurables import ToolSvc, Lcio2EDM4hepTool, EDM4hep2LcioTool

from Configurables import k4DataSvc, PodioInput
evtsvc = k4DataSvc('EventDataSvc')
evtsvc.input = "/home/kkostova/Desktop/k4GaudiPandora/simulation/sim.edm4hep.root"

inp = PodioInput('InputReader')
inp.collections = [
  'MCParticles',
  'VertexBarrelCollection',
  'VertexEndcapCollection',
  'InnerTrackerBarrelCollection',
  'OuterTrackerBarrelCollection',
  'ECalEndcapCollection',
  'ECalEndcapCollectionContributions',
  'ECalBarrelCollection',
  'ECalBarrelCollectionContributions',
  'HCalBarrelCollection',
  'HCalBarrelCollectionContributions',
  'InnerTrackerEndcapCollection',
  'OuterTrackerEndcapCollection',
  'HCalEndcapCollection',
  'HCalEndcapCollectionContributions',
  'HCalRingCollection',
  'HCalRingCollectionContributions',
  'YokeBarrelCollection',
  'YokeBarrelCollectionContributions',
  'YokeEndcapCollection',
  'YokeEndcapCollectionContributions',
  'LumiCalCollection',
  'LumiCalCollectionContributions',
]
inp.OutputLevel = WARNING



MyDDSimpleMuonDigi = MarlinProcessorWrapper("MyDDSimpleMuonDigi")
MyDDSimpleMuonDigi.OutputLevel = WARNING
MyDDSimpleMuonDigi.ProcessorType = "DDSimpleMuonDigi"
MyDDSimpleMuonDigi.Parameters = {
                                 "MUONCollections": ["ECalBarrelCollection"],
                                 "MUONOutputCollection": ["CalorimeterHit"],
                                 "RelationOutputCollection": ["RelationMuonHit"],
                                 "MuonThreshold": ["0.025"],
                                 "MuonTimeThreshold": ["0.025"],
                                 "CalibrMUON": ["120000.0"],
                                 "MaxHitEnergyMUON": ["2.0"],                              
                                 }


# EDM4hep to LCIO converter
edmConvTool = EDM4hep2LcioTool("EDM4hep2lcio")
edmConvTool.convertAll = False
edmConvTool.collNameMapping = {
    'MCParticles':                     'MCParticles',
    'VertexBarrelCollection':          'VertexBarrelCollection',
    'VertexEndcapCollection':          'VertexEndcapCollection',
    'InnerTrackerBarrelCollection':    'InnerTrackerBarrelCollection',
    'OuterTrackerBarrelCollection':    'OuterTrackerBarrelCollection',
    'InnerTrackerEndcapCollection':    'InnerTrackerEndcapCollection',
    'OuterTrackerEndcapCollection':    'OuterTrackerEndcapCollection',
    'ECalEndcapCollection':            'ECalEndcapCollection',
    'ECalBarrelCollection':            'ECalBarrelCollection',
    'HCalBarrelCollection':            'HCalBarrelCollection',
    'HCalEndcapCollection':            'HCalEndcapCollection',
    'HCalRingCollection':              'HCalRingCollection',
    'YokeBarrelCollection':            'YokeBarrelCollection',
    'YokeEndcapCollection':            'YokeEndcapCollection',
    'LumiCalCollection':               'LumiCalCollection',
}
edmConvTool.OutputLevel = DEBUG
MyDDSimpleMuonDigi.EDM4hep2LcioTool=edmConvTool


InitDD4hep = MarlinProcessorWrapper("InitDD4hep")
InitDD4hep.OutputLevel = WARNING
InitDD4hep.ProcessorType = "InitializeDD4hep"
InitDD4hep.Parameters = {
                         "DD4hepXMLFile": [os.environ["K4GEO"]+"/FCCee/CLD/compact/CLD_o2_v06/CLD_o2_v06.xml"],
                         "EncodingStringParameter": ["GlobalTrackerReadoutID"]
                         }


Output_REC = MarlinProcessorWrapper("Output_REC")
Output_REC.OutputLevel = WARNING
Output_REC.ProcessorType = "LCIOOutputProcessor"
Output_REC.Parameters = {
                         "DropCollectionNames": [],
                         "DropCollectionTypes": [],
                         "FullSubsetCollections": ["EfficientMCParticles", "InefficientMCParticles"],
                         "KeepCollectionNames": [],
                         "LCIOOutputFile": ["Output_REC.slcio"],
                         "LCIOWriteMode": ["WRITE_NEW"]
                         }


# LCIO to EDM4hep converter
lcioConvTool = Lcio2EDM4hepTool("lcio2EDM4hep")
lcioConvTool.convertAll = True
lcioConvTool.OutputLevel = DEBUG
Output_REC.Lcio2EDM4hepTool=lcioConvTool


Output_DST = MarlinProcessorWrapper("Output_DST")
Output_DST.OutputLevel = WARNING
Output_DST.ProcessorType = "LCIOOutputProcessor"
Output_DST.Parameters = {
                         "DropCollectionNames": [],
                         "DropCollectionTypes": ["MCParticle", "LCRelation", "SimCalorimeterHit", "CalorimeterHit", "SimTrackerHit", "TrackerHit", "TrackerHitPlane", "Track", "ReconstructedParticle", "LCFloatVec"],
                         "FullSubsetCollections": ["EfficientMCParticles", "InefficientMCParticles", "MCPhysicsParticles"],
                         "KeepCollectionNames": ["MCParticlesSkimmed", "MCPhysicsParticles", "RecoMCTruthLink", "SiTracks", "SiTracks_Refitted", "PandoraClusters", "PandoraPFOs", "SelectedPandoraPFOs", "LooseSelectedPandoraPFOs", "TightSelectedPandoraPFOs", "RefinedVertexJets", "RefinedVertexJets_rel", "RefinedVertexJets_vtx", "RefinedVertexJets_vtx_RP", "BuildUpVertices", "BuildUpVertices_res", "BuildUpVertices_RP", "BuildUpVertices_res_RP", "BuildUpVertices_V0", "BuildUpVertices_V0_res", "BuildUpVertices_V0_RP", "BuildUpVertices_V0_res_RP", "PrimaryVertices", "PrimaryVertices_res", "PrimaryVertices_RP", "PrimaryVertices_res_RP", "RefinedVertices", "RefinedVertices_RP"],
                         "LCIOOutputFile": ["Output_DST.slcio"],
                         "LCIOWriteMode": ["WRITE_NEW"]
                         }



# Write output to EDM4hep
from Configurables import PodioOutput
out = PodioOutput("PodioOutput", filename = "old_output_digi.root")
out.outputCommands = ["keep *"]


algList.append(inp)
algList.append(InitDD4hep)
algList.append(MyDDSimpleMuonDigi)
algList.append(Output_REC)
algList.append(Output_DST)
algList.append(out)

from Configurables import ApplicationMgr
ApplicationMgr( TopAlg = algList,
                EvtSel = 'NONE',
                EvtMax   = 3,
                ExtSvc = [evtsvc],
                OutputLevel=WARNING
              )