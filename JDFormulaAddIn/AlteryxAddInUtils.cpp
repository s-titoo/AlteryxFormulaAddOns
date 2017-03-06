#include "stdafx.h"
#include "AlteryxAddInUtils.h"


void AlteryxAddInUtils::DoSetString(FormulaAddInData* pReturnValue, const wchar_t* pString)
{
	size_t nLen = wcslen(pString);
	auto *pStringRet = static_cast<wchar_t *>(GlobalAlloc(GMEM_FIXED, (nLen + 1) * sizeof(wchar_t)));
	wcscpy_s(pStringRet, nLen + 1, pString);
	pReturnValue->pVal = pStringRet;
}

void AlteryxAddInUtils::DoResetNull(int nNumArgs, FormulaAddInData* pArgs)
{
	for (int x = 0; x < nNumArgs; x++)
	{
		pArgs[x].isNull = 0;
	}
}

void AlteryxAddInUtils::SetString(FormulaAddInData* pReturnValue, const wchar_t *pString)
{
	DoSetString(pReturnValue, pString);
	pReturnValue->nVarType = 2;
}

long AlteryxAddInUtils::ReturnError(const wchar_t *pString, FormulaAddInData* pReturnValue, int nNumArgs, FormulaAddInData* pArgs) {
	pReturnValue->isNull = 1;
	DoSetString(pReturnValue, pString);
	DoResetNull(nNumArgs, pArgs);
	return 0;
}

long AlteryxAddInUtils::ReturnSuccess(int nNumArgs, FormulaAddInData *pArgs) {
	DoResetNull(nNumArgs, pArgs);
	return 1;
}

void AlteryxAddInUtils::CopyValue(const FormulaAddInData *source, FormulaAddInData *target) {
	target->nVarType = source->nVarType;

	if (!source->isNull) {
		target->isNull = 0;
		if (1 == source->nVarType) {
			target->dVal = source->dVal;
		}
		else {
			SetString(target, source->pVal);
		}
	}
	else {
		target->isNull = 1;
	}
}