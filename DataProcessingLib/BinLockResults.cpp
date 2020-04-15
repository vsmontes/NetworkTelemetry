/* Rev 27/02/2019 */
#include "BinLockResults.h"
#include "BinProcessor.h"

CBinLockResults::CBinLockResults()
{
	CBinPktProcessor::results_mtx.lock();
}

CBinLockResults::~CBinLockResults()
{
	CBinPktProcessor::results_mtx.unlock();
}
