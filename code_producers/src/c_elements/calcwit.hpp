#ifndef CIRCOM_CALCWIT_H
#define CIRCOM_CALCWIT_H

#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <memory>

#include "circom.hpp"
#include "fr.hpp"

#define NMUTEXES 12 //512

u64 fnv1a(std::string s);

class Circom_CalcWit {

  bool *inputSignalAssigned;
  uint inputSignalAssignedCounter;

  Circom_Circuit *circuit;

public:

  FrElement *signalValues;
  Circom_Component* componentMemory;
  FrElement* circuitConstants; 
  std::map<u32,IODefPair> templateInsId2IOSignalInfo; 
  std::string* listOfTemplateMessages; 

  // parallelism
  std::mutex numThreadMutex;
  std::condition_variable ntcvs;
  uint numThread;

  uint maxThread;

  // Functions called by the circuit
  Circom_CalcWit(Circom_Circuit *aCircuit, uint numTh = NMUTEXES);
  ~Circom_CalcWit();

  // Public functions
  void setInputSignal(u64 h, uint i, FrElement &val);
  
  uint getWitnessSize();
  
  inline void getWitness(uint idx, PFrElement val) {
    Fr_copy(val, &signalValues[circuit->witness2SignalList[idx]]);
  }

  std::string getTrace(u64 id_cmp);

  std::string generate_position_array(uint* dimensions, uint size_dimensions, uint index);
};

typedef void (*Circom_TemplateFunction)(uint __cIdx, Circom_CalcWit* __ctx); 

#endif // CIRCOM_CALCWIT_H
