#include "varsDict.hpp"
#include "definitions.hpp"
#include <iostream>


    int VarsDict::addVariable(Variable* variable) {
        variable->id =1 + 2 * lookupTable.size() ;
        lookupTable.push_back(variable);
        int exampleId = variable->exampleId;
        int timestep = variable->timestep;
        int skeletonId = variable->skeletonId;
        if (variable->varType == VarType::ETS){
            if (examples.find(exampleId) == examples.end()){
                unordered_map<int, unordered_map<int,Variable*> > timestepMap;
                examples[exampleId] = timestepMap;
            }
            if (examples[exampleId].find(variable->timestep) == examples[exampleId].end()){
                unordered_map<int,Variable*> variables; 
                examples[exampleId][variable->timestep] = variables;
            }
            examples[exampleId][timestep][skeletonId] = variable;
            
        }
        else if (variable->varType == VarType::ST){
            if (skeletons.find(skeletonId) == skeletons.end()){
                unordered_map<int,Variable*> variables; 
                skeletons[skeletonId] = variables; 
            }
            skeletons[skeletonId][variable->skeletonType] = variable;
    
        }
        else if (variable->varType == VarType::alpha){
            if (alphas.find(skeletonId) == alphas.end()){
                unordered_map<int,Variable*> variables; 
                alphas[skeletonId] = variables; 
            }
            alphas[skeletonId][variable->alpha] = variable;
        }
        else if (variable->varType == VarType::beta){
            if (betas.find(skeletonId) == betas.end()){
                unordered_map<int,Variable*> variables; 
                betas[skeletonId] = variables; 
            }
            //cout << "added beta ... for skId " << skeletonId << " and beta " << variable->beta   << endl;
            betas[skeletonId][variable->beta] = variable;
    
        }
        else if (variable->varType == VarType::skliteral){
            if (skliterals.find(skeletonId) == skliterals.end()){
                unordered_map<int,Variable*> variables; 
                skliterals[skeletonId] = variables; 
            }
            skliterals[skeletonId][variable->literal] = variable;
        }
        else if (variable->varType == VarType::metaETS){
            if (metaETS.find(exampleId) == metaETS.end()){
                unordered_map<int, unordered_map<int,unordered_map<int,Variable*> > > timestepMap;
                metaETS[exampleId] = timestepMap;
            }
            if (metaETS[exampleId].find(variable->timestep) == metaETS[exampleId].end()){
                unordered_map<int,unordered_map<int,Variable*> > variables; 
                metaETS[exampleId][variable->timestep] = variables;
            }
            if (metaETS[exampleId][timestep].find(skeletonId) == metaETS[exampleId][timestep].end()){
                unordered_map<int,Variable*> map;
                metaETS[exampleId][timestep][skeletonId] = map;
            }
            //alpha is here the position in the internal mapping inside of the meta operator
            metaETS[exampleId][timestep][skeletonId][variable->alpha] = variable;
        } 
        else if (variable->varType == VarType::metaST){
            if (metaSkeletons.find(skeletonId) == metaSkeletons.end()){
                unordered_map<int,Variable*> variables; 
                metaSkeletons[skeletonId] = variables; 
            }
            metaSkeletons[skeletonId][variable->skeletonType] = variable;
        }

        return variable->id;
    }

    int VarsDict::getVarEtsId(int exampleId, int timestep, int skeletonId){
        return examples[exampleId][timestep][skeletonId]->id;
    }

    int VarsDict::getMetaEtsId(int exampleId, int timestep, int skeletonId, int metaPos){
        return metaETS[exampleId][timestep][skeletonId][metaPos]->id;
    }

    int VarsDict::getMetaStId(int skeletonId, int skeletonType){
        return metaSkeletons[skeletonId][skeletonType]->id;
    }
    int VarsDict::getVarSTId(int skeletonId, int skeletonType){
       return skeletons[skeletonId][skeletonType]->id;
    }

    int VarsDict::getVarEtlId(int exampleId,int timestep, int literal){
        return examples[exampleId][timestep][literal]->id;
    }


    int VarsDict::getVarAlphaId(int skeletonId, int alphaSkeletonId){
        return alphas[skeletonId][alphaSkeletonId]->id;
    }

    int VarsDict::getVarBetaId(int skeletonId, int betaSkeletonId){
        return betas[skeletonId][betaSkeletonId]->id;
    }

    int VarsDict::getVarLitId(int skeletonId, int literal){
        return skliterals[skeletonId][literal]->id;
    }
