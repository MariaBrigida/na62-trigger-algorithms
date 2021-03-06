/*
 * TriggerProcessor.h
 *
 *  Created on: Nov 17, 2011
 *      Author: Jonas Kunze (kunze.jonas@gmail.com)
 */

#pragma once
#ifndef TRIGGERPROCESSOR_H_
#define TRIGGERPROCESSOR_H_

#include <sys/types.h>
#include <random>
#include <cstdint>
#include <atomic>

#include "L1InfoToStorage.h"
#include "../options/TriggerOptions.h"
#include "../struct/HLTConfParams.h"
#include "L1Downscaling.h"
#include "L1Reduction.h"

namespace na62 {

class EventBuilder;
class Event;
//class CedarData;
class KtagAlgo;
class RICHAlgo;
class CHODAlgo;

class L1TriggerProcessor {
public:
	/**
	 * @param event Event* This is a pointer to the built Event containing all subevents (except those from the LKr)
	 *
	 * The event pointer may not be deleted by this method as it is deleted by the EventBuilder
	 *
	 * @return uint_fast8_t <0> if the event is rejected, the L1 trigger type word in other cases.
	 */

	static uint_fast8_t compute(Event* const event);

	/**
	 * Returns true if the current event should be bypassed instead of being processed
	 */
	static inline bool bypassEvent() {
		if (bypassProbability == 0.0) {
			return false;
		}
		double randomNr = ((double) rand() / (double) RAND_MAX);
		return randomNr <= bypassProbability;
	}

	/**
	 * Registers all downscaling algorithms. Must be called before Options::Load is executed!
	 */
	static void registerDownscalingAlgorithms();

	/**
	 * Registers all reduction algorithms. Must be called before Options::Load is executed!
	 */
	static void registerReductionAlgorithms();

	/**
	 * Placeholder for deciding whether or not to request ZS CREAM data
	 */
	static bool isRequestZeroSuppressedCreamData(
			uint_fast8_t l1TriggerTypeWord);

	static inline std::atomic<uint64_t>* GetL1TriggerStats() {
		return L1Triggers_;
	}
	static inline uint64_t GetL1InputStats() {
		return L1InputEvents_;
	}
	static inline uint64_t GetL1InputReceivedStats() {
		return L1InputReducedEvents_;
	}
	static inline uint64_t GetL1InputEventsPerBurst() {
		return L1InputEventsPerBurst_;
	}
	static void ResetL1InputEventsPerBurst() {
		L1InputEventsPerBurst_ = 0;
	}
	static inline uint64_t GetL1BypassedEvents() {
		return L1BypassedEvents_;
	}
	static inline uint GetL1DownscaleFactor() {
		return downscaleFactor;
	}
	static inline uint GetL1ReductionFactor() {
		return reductionFactor;
	}
	static inline bool GetL1FlagMode() {
		return flagMode;
	}
	static inline uint GetL1AutoFlagFactor() {
		return autoFlagFactor;
	}

	static void initialize(l1Struct &l1Struct);

private:
	static std::atomic<uint64_t>* L1Triggers_;
	static std::atomic<uint64_t>* L1AcceptedEventsPerL0Mask_;
	static std::atomic<uint64_t>** eventCountersByL0MaskByAlgoID_;
	static std::atomic<uint64_t> L1BypassedEvents_;
	static std::atomic<uint64_t> L1InputEvents_;
	static std::atomic<uint64_t> L1InputReducedEvents_;
	static std::atomic<uint64_t> L1InputEventsPerBurst_;
	static std::atomic<uint64_t> L1AcceptedEvents_;

	//Global L1 configuration parameters (for all L0 masks)
	static double bypassProbability;
	static uint reductionFactor;
	static uint downscaleFactor;
	static bool flagMode;
	static uint autoFlagFactor;
	static uint referenceTimeSourceID;

	// L1 Mask configuration parameters (for 16 L0 masks)
	static uint numberOfEnabledAlgos[16];
	static uint numberOfFlaggedAlgos[16];
	static uint algoReductionFactor[16];

	static uint_fast16_t algoEnableMask[16];
	static uint_fast16_t algoDwScMask[16];
	static uint algoDwScFactor[16][4];

	static uint_fast16_t chodEnableMask;
	static uint_fast16_t richEnableMask;
	static uint_fast16_t cedarEnableMask;
	static uint_fast16_t lavEnableMask;

	static uint_fast16_t chodFlagMask;
	static uint_fast16_t richFlagMask;
	static uint_fast16_t cedarFlagMask;
	static uint_fast16_t lavFlagMask;

	static int chodProcessID[16]; //default values = -1
	static int richProcessID[16];
	static int cedarProcessID[16];
	static int lavProcessID[16];

	// Downscaling variables
	static uint chodAlgorithmId;
	static uint richAlgorithmId;
	static uint cedarAlgorithmId;
	static uint lavAlgorithmId;

	// Varie
	static uint_fast8_t l0TrigWord;   //=1 in 2015
	static uint_fast16_t l0TrigFlags; //16 bit word: bit ith set to 0(1) if L0 mask ith has(has NOT) triggered!

	static uint_fast8_t chodTrigger;
	static uint_fast8_t richTrigger;
	static uint_fast8_t cedarTrigger;
	static uint_fast8_t lavTrigger;
	static uint_fast8_t l1TriggerWords[16];

	static L1InfoToStorage* l1Info_;
	static uint l1ProcessID;
	static uint numberOfTriggeredL1Masks;
	static bool isAlgoEnableForAllL0Masks;
	static bool isDownscaledAndFlaggedEvent;
	static bool isReducedEvent;
	static bool isAllL1AlgoDisable;
}
;

} /* namespace na62 */
#endif /* TRIGGERPROCESSOR_H_ */
