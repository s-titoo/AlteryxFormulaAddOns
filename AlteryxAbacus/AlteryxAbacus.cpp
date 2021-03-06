#include "stdafx.h"
#include "AlteryxAbacus.h"
#include "AlteryxAbacusUtils.h"
#include <string>
#include <deque>
#include <map>
#include <sstream>
#include <ostream>
#include <fstream>

static std::map<std::wstring, std::wstring> stringLookup;
static std::map<std::wstring, double> doubleLookup;
static std::map<std::wstring, std::deque<double>> listLookup;

static size_t get_index(const size_t size, double dVal)
{
	if (dVal < 0)
	{
		const auto shift = static_cast<size_t>(-dVal);
		return shift > size ? size : size - shift;
	}

	return static_cast<size_t>(dVal);
}

extern "C" long _declspec(dllexport) _stdcall VarTextExists(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
	if (nNumArgs < 1 || nNumArgs > 2 || pArgs[0].nVarType == 1)
	{
		return AlteryxAbacusUtils::ReturnError(L"VarTextExists: Syntax error - key:string argument.", pReturnValue, nNumArgs, pArgs);
	}

	pReturnValue->nVarType = 1;
	if (pArgs[0].isNull)
	{
		pReturnValue->isNull = true;
	}
	else
	{
		const std::wstring key(pArgs[0].pVal);
		pReturnValue->isNull = false;
		const auto stringFind = stringLookup.find(key);
		pReturnValue->dVal = stringFind != stringLookup.end();
	}

	return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
}

extern "C" long _declspec(dllexport) _stdcall VarNumExists(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
	if (nNumArgs < 1 || nNumArgs > 2 || pArgs[0].nVarType == 1)
	{
		return AlteryxAbacusUtils::ReturnError(L"VarNumExists: Syntax error - key:string argument.", pReturnValue, nNumArgs, pArgs);
	}

	pReturnValue->nVarType = 1;
	if (pArgs[0].isNull)
	{
		pReturnValue->isNull = true;
	}
	else
	{
		const std::wstring key(pArgs[0].pVal);
		pReturnValue->isNull = false;
		const auto doubleFind = doubleLookup.find(key);
		pReturnValue->dVal = doubleFind != doubleLookup.end();
	}

	return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
}

extern "C" long _declspec(dllexport) _stdcall VarListExists(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
	if (nNumArgs < 1 || nNumArgs > 2 || pArgs[0].nVarType == 1)
	{
		return AlteryxAbacusUtils::ReturnError(L"VarListExists: Syntax error - key:string argument.", pReturnValue, nNumArgs, pArgs);
	}

	pReturnValue->nVarType = 1;
	if (pArgs[0].isNull)
	{
		pReturnValue->isNull = true;
	}
	else
	{
		const std::wstring key(pArgs[0].pVal);
		pReturnValue->isNull = false;
		const auto listFind = listLookup.find(key);
		pReturnValue->dVal = listFind != listLookup.end();
	}

	return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
}

extern "C" long _declspec(dllexport) _stdcall VarText(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
	if (nNumArgs < 1 || pArgs[0].nVarType == 1 || (nNumArgs > 1 && pArgs[1].nVarType != 2) || (nNumArgs > 2 && pArgs[2].nVarType != 1) || nNumArgs > 3)
	{
		return AlteryxAbacusUtils::ReturnError(L"VarText: Syntax error - key:string [value:string] [setIfTrue:boolean = true] arguments.", pReturnValue, nNumArgs, pArgs);
	}

	pReturnValue->nVarType = 2;
	if (pArgs[0].isNull)
	{
		pReturnValue->isNull = true;
	}
	else
	{
		const std::wstring key(pArgs[0].pVal);

		if (nNumArgs > 1 && (nNumArgs < 3 || pArgs[2].isNull || pArgs[2].dVal != 0))
		{
			if (pArgs[1].isNull)
			{
				stringLookup.erase(key);
			}
			else
			{
				const std::wstring value(pArgs[1].pVal);
				stringLookup[key] = value;
			}
		}

		const auto stringFind = stringLookup.find(key);
		if (stringFind != stringLookup.end())
		{
			AlteryxAbacusUtils::SetString(pReturnValue, stringFind->second.c_str());
		}
		else
		{
			pReturnValue->isNull = true;
		}
	}

	return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
}

extern "C" long _declspec(dllexport) _stdcall VarNum(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
	if (nNumArgs < 1 || pArgs[0].nVarType == 1 || (nNumArgs > 1 && pArgs[1].nVarType != 1) || (nNumArgs > 2 && pArgs[2].nVarType != 1) || nNumArgs > 3)
	{
		return AlteryxAbacusUtils::ReturnError(L"VarNum: Syntax error - key:string [value:number] [setIfTrue:boolean = true] arguments.", pReturnValue, nNumArgs, pArgs);
	}

	pReturnValue->nVarType = 1;
	if (pArgs[0].isNull)
	{
		pReturnValue->isNull = true;
	}
	else
	{
		const std::wstring key(pArgs[0].pVal);

		if (nNumArgs > 1 && (nNumArgs < 3 || pArgs[2].isNull || pArgs[2].dVal != 0))
		{
			if (pArgs[1].isNull)
			{
				doubleLookup.erase(key);
			}
			else
			{
				doubleLookup[key] = pArgs[1].dVal;
			}
		}

		const auto doubleFind = doubleLookup.find(key);
		if (doubleFind != doubleLookup.end())
		{
			pReturnValue->dVal = doubleFind->second;
		}
		else
		{
			pReturnValue->isNull = true;
		}
	}

	return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
}

extern "C" long _declspec(dllexport) _stdcall VarListLength(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
	if (nNumArgs < 1 || nNumArgs > 2 || pArgs[0].nVarType != 2)
	{
		return AlteryxAbacusUtils::ReturnError(L"VarListLength: Syntax error - key:string argument.", pReturnValue, nNumArgs, pArgs);
	}

	pReturnValue->nVarType = 1;
	pReturnValue->isNull = true;
	if (!pArgs[0].isNull)
	{
		const std::wstring key(pArgs[0].pVal);
		const auto listFind = listLookup.find(key);
		if (listFind != listLookup.end())
		{
			pReturnValue->isNull = false;
			pReturnValue->dVal = static_cast<double>(listFind->second.size());
		}
	}

	return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
}

extern "C" long _declspec(dllexport) _stdcall VarListGet(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
	if (nNumArgs != 2 || pArgs[0].nVarType != 2 || pArgs[1].nVarType != 1)
	{
		return AlteryxAbacusUtils::ReturnError(L"VarListGet: Syntax error - key:string index:integer arguments.", pReturnValue, nNumArgs, pArgs);
	}

	pReturnValue->nVarType = 1;
	pReturnValue->isNull = true;
	if (!pArgs[0].isNull && !pArgs[1].isNull)
	{
		const std::wstring key(pArgs[0].pVal);
		const auto listFind = listLookup.find(key);
		if (listFind != listLookup.end())
		{
			const auto size = listFind->second.size();
			const auto index = get_index(size, pArgs[1].dVal);
			if (index >= 0 && index < size) 
			{
				pReturnValue->isNull = false;
				pReturnValue->dVal = listFind->second[index];
			}
		}
	}

	return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
}

extern "C" long _declspec(dllexport) _stdcall VarListAdd(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
	if (nNumArgs < 2 || nNumArgs > 3 || pArgs[0].nVarType != 2 || pArgs[1].nVarType != 1 || (nNumArgs > 2 && pArgs[2].nVarType != 1))
	{
		return AlteryxAbacusUtils::ReturnError(L"VarListAdd: Syntax error - key:string [value:number] [setIfTrue:boolean = true] arguments.", pReturnValue, nNumArgs, pArgs);
	}

	pReturnValue->nVarType = 1;
	pReturnValue->isNull = true;

	if (nNumArgs == 3 && !pArgs[2].isNull && pArgs[2].dVal == 0)
	{
		return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
	}

	if (!pArgs[0].isNull && !pArgs[1].isNull)
	{
		const std::wstring key(pArgs[0].pVal);
		const auto listFind = listLookup.find(key);
		if (listFind != listLookup.end())
		{
			listFind->second.push_back(pArgs[1].dVal);
		}
		else
		{
			const std::deque<double> vector({ pArgs[1].dVal });
			listLookup[key] = vector;
		}

		pReturnValue->isNull = false;
		pReturnValue->dVal = pArgs[1].dVal;
	}

	return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
}

extern "C" long _declspec(dllexport) _stdcall VarListSet(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
	if (nNumArgs < 3 || nNumArgs > 4 || pArgs[0].nVarType != 2 || pArgs[1].nVarType != 1 || pArgs[2].nVarType != 1 || (nNumArgs == 4 && pArgs[3].nVarType != 1))
	{
		return AlteryxAbacusUtils::ReturnError(L"VarListSet: Syntax error - key:string index:integer value:number [setIfTrue:boolean = true] arguments.", pReturnValue, nNumArgs, pArgs);
	}

	pReturnValue->nVarType = 1;
	pReturnValue->isNull = true;

	if (nNumArgs == 4 && !pArgs[3].isNull && pArgs[3].dVal == 0)
	{
		return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
	}

	if (!pArgs[0].isNull && !pArgs[1].isNull && !pArgs[2].isNull)
	{
		const std::wstring key(pArgs[0].pVal);
		const auto listFind = listLookup.find(key);
		if (listFind != listLookup.end())
		{
			const auto size = listFind->second.size();
			const auto index = get_index(size, pArgs[1].dVal);
			if (index < 0 || index >= size)
			{
				return AlteryxAbacusUtils::ReturnError(L"Cannot set as index out of range", pReturnValue, nNumArgs, pArgs);
			}
			listFind->second[index] = pArgs[2].dVal;
		}
		else
		{
			return AlteryxAbacusUtils::ReturnError(L"Cannot set as index out of range", pReturnValue, nNumArgs, pArgs);
		}

		pReturnValue->isNull = false;
		pReturnValue->dVal = pArgs[2].dVal;
	}

	return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
}

extern "C" long _declspec(dllexport) _stdcall VarListInsert(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
	if (nNumArgs < 3 || nNumArgs > 4 || pArgs[0].nVarType != 2 || pArgs[1].nVarType != 1 || pArgs[2].nVarType != 1 || (nNumArgs == 4 && pArgs[3].nVarType != 1))
	{
		return AlteryxAbacusUtils::ReturnError(L"VarListInsert: Syntax error - key:string index:integer value:number [setIfTrue:boolean = true] arguments.", pReturnValue, nNumArgs, pArgs);
	}

	pReturnValue->nVarType = 1;
	pReturnValue->isNull = true;

	if (nNumArgs == 4 && !pArgs[3].isNull && pArgs[3].dVal == 0)
	{
		return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
	}
	
	if (!pArgs[0].isNull && !pArgs[1].isNull && !pArgs[2].isNull)
	{
		const std::wstring key(pArgs[0].pVal);
		const auto listFind = listLookup.find(key);
		if (listFind != listLookup.end())
		{
			const auto size = listFind->second.size();
			const auto index = get_index(size, pArgs[1].dVal);
			if (index < 0 || index > size)
			{
				return AlteryxAbacusUtils::ReturnError(L"Cannot insert as index out of range", pReturnValue, nNumArgs, pArgs);
			}
			listFind->second.insert(listFind->second.begin() + index, pArgs[2].dVal);
		}
		else
		{
			if (pArgs[1].dVal != 0)
			{
				return AlteryxAbacusUtils::ReturnError(L"Cannot insert as index out of range", pReturnValue, nNumArgs, pArgs);
			}
			const std::deque<double> vector({ pArgs[2].dVal });
			listLookup[key] = vector;
		}

		pReturnValue->isNull = false;
		pReturnValue->dVal = pArgs[2].dVal;
	}

	return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
}

extern "C" long _declspec(dllexport) _stdcall VarListRemove(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
	if (nNumArgs < 2 || nNumArgs > 3 || pArgs[0].nVarType != 2 || pArgs[1].nVarType != 1 || (nNumArgs == 3 && pArgs[2].nVarType != 1))
	{
		return AlteryxAbacusUtils::ReturnError(L"VarListRemove: Syntax error - key:string index:integer [setIfTrue:boolean = true] arguments.", pReturnValue, nNumArgs, pArgs);
	}

	pReturnValue->nVarType = 1;
	pReturnValue->isNull = true;

	if (nNumArgs == 3 && !pArgs[2].isNull && pArgs[2].dVal == 0)
	{
		return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
	}
 
	if (!pArgs[0].isNull && !pArgs[1].isNull)
	{
		const std::wstring key(pArgs[0].pVal);
		const auto listFind = listLookup.find(key);
		if (listFind != listLookup.end())
		{
			const auto size = listFind->second.size();
			const auto index = get_index(size, pArgs[1].dVal);
			if (index >= 0 && index < size)
			{
				const auto it = listFind->second.begin() + index;
				pReturnValue->isNull = false;
				pReturnValue->dVal = *it;
				listFind->second.erase(it);
			}
		}
	}

	return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
}

extern "C" long _declspec(dllexport) _stdcall VarReset(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
	if (nNumArgs > 1 || (nNumArgs == 1 && pArgs[0].nVarType == 1))
	{
		return AlteryxAbacusUtils::ReturnError(L"VarReset: Takes an optional string key argument.", pReturnValue, nNumArgs, pArgs);
	}

	pReturnValue->nVarType = 1;
	pReturnValue->isNull = true;

	if (nNumArgs == 1)
	{
		if (!pArgs[0].isNull) {
			const std::wstring key(pArgs[0].pVal);
			doubleLookup.erase(key);
			stringLookup.erase(key);
			listLookup.erase(key);
		}
	}
	else {
		stringLookup.clear();
		doubleLookup.clear();
		listLookup.clear();
	}
	
	return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
}
	
extern "C" long _declspec(dllexport) _stdcall VarPrint(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
	pReturnValue->nVarType = 2;
	pReturnValue->isNull = false;

	std::wstringstream buffer;

	for (const auto& pair : stringLookup )
	{
		buffer << "text\t";
		buffer << pair.first;
		buffer << "\t";
		buffer << pair.second;
		buffer << "\n";
	}

	for (const auto& pair : doubleLookup)
	{
		buffer << "text\t";
		buffer << pair.first;
		buffer << "\t";
		buffer << pair.second;
		buffer << "\n";
	}

	for (const auto& pair : listLookup)
	{
		buffer << "text\t";
		buffer << pair.first;
		buffer << "\t";

		bool first = true;
		for (const auto value : pair.second)
		{
			if (!first)
			{
				buffer << ",";
			}
			buffer << value;
			first = false;
		}
		buffer << "\n";
	}

	AlteryxAbacusUtils::SetString(pReturnValue, buffer.str().c_str());
	return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
}

extern "C" long _declspec(dllexport) _stdcall LogToFile(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
	if (!pArgs[1].isNull && !pArgs[2].isNull)
	{
		std::wofstream fout(pArgs[1].pVal, std::fstream::in | std::fstream::out | std::fstream::app);
		fout << pArgs[2].pVal;
		fout << "\n";
		fout.flush();
		fout.close();
	}

	pReturnValue->nVarType = pArgs[0].nVarType;
	AlteryxAbacusUtils::CopyValue(&pArgs[0], pReturnValue);
	return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
}

//// a way to error a function from XML
extern "C" long _declspec(dllexport) _stdcall ReportError(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
	pReturnValue->nVarType = nNumArgs > 2 ? pArgs[2].nVarType : 1;

	if (nNumArgs < 1 || pArgs[0].nVarType != 1 || pArgs[0].isNull != 0 || pArgs[0].dVal != 0) {
		return AlteryxAbacusUtils::ReturnError((nNumArgs < 2 || pArgs[1].nVarType == 1) ? L"Reporting An Error!" : pArgs[1].pVal, pReturnValue, nNumArgs, pArgs);
	}

	if (nNumArgs < 2) {
		pReturnValue->isNull = 1;
	}
	else {
		AlteryxAbacusUtils::CopyValue(&pArgs[2], pReturnValue);
	}

	return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
}

//// this sample takes a variable number of inputs and returns the first non-null
extern "C" long _declspec(dllexport) _stdcall Coalesce(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
	if (nNumArgs < 1) {
		pReturnValue->nVarType = 1;
		return AlteryxAbacusUtils::ReturnError(L"Need an argument!", pReturnValue, nNumArgs, pArgs);
	}

	int varType = pArgs[0].nVarType;
	pReturnValue->nVarType = varType;

	pReturnValue->isNull = 1;

	for (int x = 0; x < nNumArgs; x++)
	{
		if (pArgs[x].nVarType != varType) {
			return AlteryxAbacusUtils::ReturnError(L"Mismatched argument types, all must be same general type as first parameter.", pReturnValue, nNumArgs, pArgs);
		}

		if (!pArgs[x].isNull)
		{
			AlteryxAbacusUtils::CopyValue(&pArgs[x], pReturnValue);
			break;
		}
	}

	return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
}

// this sample takes a variable number of inputs and returns the first non-null
extern "C" long _declspec(dllexport) _stdcall Count(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
	pReturnValue->nVarType = 1;
	pReturnValue->isNull = 0;

	int count = 0;
	for (int x = 0; x < nNumArgs; x++)
	{
		count += pArgs[x].isNull == 0;
	}

	pReturnValue->dVal = count;
	return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
}

// this sample takes a variable number of inputs and returns the first non-null
extern "C" long _declspec(dllexport) _stdcall Sum(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
	pReturnValue->nVarType = 1;
	pReturnValue->isNull = 0;

	double sum = 0;
	for (int x = 0; x < nNumArgs; x++)
	{
		if (pArgs[x].nVarType != 1) {
			return AlteryxAbacusUtils::ReturnError(L"Non-numeric argument, all must be numbers.", pReturnValue, nNumArgs, pArgs);
		}

		if (!pArgs[x].isNull)
		{
			sum += pArgs[x].dVal;
		}
	}

	pReturnValue->dVal = sum;
	return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
}

// this sample takes a variable number of inputs and returns the first non-null
extern "C" long _declspec(dllexport) _stdcall Avg(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
	pReturnValue->nVarType = 1;
	pReturnValue->isNull = 0;

	double sum = 0;
	int count = 0;
	for (int x = 0; x < nNumArgs; x++)
	{
		if (pArgs[x].nVarType != 1)
		{
			const wchar_t* errorMessage = L"Non-numeric argument, all must be numbers.";
			AlteryxAbacusUtils::SetString(pReturnValue, errorMessage);
			pReturnValue->isNull = 1;
			return 0;
		}

		if (pArgs[x].isNull == 0)
		{
			sum += pArgs[x].dVal;
			count++;
		}
	}

	if (count == 0) {
		pReturnValue->isNull = 1;
	}
	else {
		pReturnValue->dVal = sum / count;
	}

	return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
}

auto matches(const wchar_t letter, const std::wstring separator) {
	for (auto i : separator)
	{
		if (i == letter) {
			return true;
		}
	}

	return false;
}

// Need String, String (char), Integer
extern "C" long _declspec(dllexport) _stdcall Split(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
	pReturnValue->nVarType = 2;

	// Check Input Parameters
	if (nNumArgs != 3 ||
		pArgs[0].nVarType != 2 ||
		pArgs[1].nVarType != 2 ||
		pArgs[2].nVarType != 1) {
		return AlteryxAbacusUtils::ReturnError(L"Syntax: String, Delimiter, Token Number.", pReturnValue, nNumArgs, pArgs);
	}

	// Check for Nulls
	if (pArgs[0].isNull || pArgs[2].isNull || pArgs[2].dVal < 1 || pArgs[1].isNull) {
		AlteryxAbacusUtils::CopyValue(&pArgs[0], pReturnValue);
		return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
	}

	// Separator Map
	const std::wstring separatorString(pArgs[1].pVal);

	// Copy the string
	const std::wstring input(pArgs[0].pVal);
	const size_t len = input.size();
	if (!len) {
		AlteryxAbacusUtils::CopyValue(&pArgs[0], pReturnValue);
		return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
	}

	int count = static_cast<int>(pArgs[2].dVal);
	pReturnValue->isNull = 1;
	size_t oldPos = 0;
	size_t charPos;
	for (charPos = 0; charPos < len; charPos++) {
		if (matches(input[charPos], separatorString)) {
			if (!--count) {
				break;
			}

			oldPos = charPos + 1;
		}
	}

	if (count <= 1) {
		AlteryxAbacusUtils::SetString(pReturnValue, oldPos < charPos ? input.substr(oldPos, charPos - oldPos).c_str() : L"");
		pReturnValue->isNull = 0;
	}

	return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
}

// Need String, String (char), Integer
extern "C" long _declspec(dllexport) _stdcall RangeJoin(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
	pReturnValue->nVarType = 1;

	// Check Input Parameters
	if (nNumArgs != 2 ||
		pArgs[0].nVarType != 1 ||
		pArgs[1].nVarType != 2) {
		return AlteryxAbacusUtils::ReturnError(L"Syntax: Number, RangeList", pReturnValue, nNumArgs, pArgs);
	}

	// Check for Nulls
	if (pArgs[0].isNull || pArgs[1].isNull) {
		pReturnValue->isNull = 1;
		return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
	}

	// Read Target Value to Long Long
	const auto target = static_cast<long long>(pArgs[0].dVal);

	// Read String
	int done_one = 0;
	const wchar_t* start = pArgs[1].pVal;
	while (*start != L'\0') {
		wchar_t* end;
		const auto value = _wcstoi64(start + done_one, &end, 10);
		done_one = 1;

		start = end;
		if (value > target)
		{
			pReturnValue->dVal = static_cast<double>(value);
			return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
		}
	}

	pReturnValue->isNull = 1;
	return AlteryxAbacusUtils::ReturnSuccess(nNumArgs, pArgs);
}