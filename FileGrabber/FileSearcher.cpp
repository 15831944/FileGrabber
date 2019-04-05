#include "stdafx.h"
#include "FileSearcher.h"
#include <regex>
using namespace std;

#ifdef UNICODE
typedef wregex _tregex;
#else
typedef regex _tregex;
#endif

shared_ptr<list<FileData>> FileSearcher::Find(shared_ptr<list<FileData>> plist, vector<_tstring> filters, bool icase) const
{
	shared_ptr<list<FileData>> nlist = make_shared<list<FileData>>();
	for (const FileData& data : *plist) {
		for (const _tstring& str : filters) {
			if (icase) {
				if (_tcsicmp(data.name, str.c_str()) == 0) {
					nlist->push_back(data);
				}
			}
			else {
				if (_tcscmp(data.name, str.c_str()) == 0) {
					nlist->push_back(data);
				}
			}
		}
	}
	return nlist;
}

shared_ptr<list<FileData>> FileSearcher::FindRegex(shared_ptr<list<FileData>> plist, vector<_tstring> regexStrs) const
{
	shared_ptr<list<FileData>> nlist = make_shared<list<FileData>>();
	vector<_tregex> regs;
	for (const _tstring& str : regexStrs) {
		regs.push_back(_tregex(str));
	}
	for (const FileData& data : *plist) {
		for (const _tregex& reg : regs) {
			if (regex_match(data.name, reg))
				nlist->push_back(data);
		}
	}
	return nlist;
}

shared_ptr<list<FileData>> FileSearcher::FindDirectory(shared_ptr<list<FileData>> plist, vector<_tstring> filters, bool icase) const
{
	shared_ptr<list<FileData>> nlist = make_shared<list<FileData>>();
	for (const FileData& data : *plist) {
		if (data.attributes & FILE_ATTRIBUTE_DIRECTORY) {
			for (const _tstring& str : filters) {
				if (icase) {
					if (_tcsicmp(data.name, str.c_str()) == 0)
						nlist->push_back(data);
				}
				else {
					if (_tcscmp(data.name, str.c_str()) == 0)
						nlist->push_back(data);
				}
			}
		}
	}
	return nlist;
}

shared_ptr<list<FileData>> FileSearcher::FindDirectoryRegex(shared_ptr<list<FileData>> plist, vector<_tstring> regexStrs) const
{
	shared_ptr<list<FileData>> nlist = make_shared<list<FileData>>();
	vector<_tregex> regs;
	for (const _tstring& str : regexStrs) {
		regs.push_back(_tregex(str));
	}
	for (const FileData& data : *plist) {
		if (data.attributes & FILE_ATTRIBUTE_DIRECTORY) {
			for (const _tregex& reg : regs) {
				if (regex_match(data.name, reg))
					nlist->push_back(data);
			}
		}
	}
	return nlist;
}

shared_ptr<list<FileData>> FileSearcher::FindFile(shared_ptr<list<FileData>> plist, vector<_tstring> filters, bool icase) const
{
	shared_ptr<list<FileData>> nlist = make_shared<list<FileData>>();
	for (const FileData& data : *plist) {
		if (!(data.attributes & FILE_ATTRIBUTE_DIRECTORY)) {
			for (const _tstring& str : filters) {
				if (icase) {
					if (_tcsicmp(data.name, str.c_str()) == 0)
						nlist->push_back(data);
				}
				else {
					if (_tcscmp(data.name, str.c_str()) == 0)
						nlist->push_back(data);
				}
			}
		}
	}
	return nlist;
}

shared_ptr<list<FileData>> FileSearcher::FindFileRegex(shared_ptr<list<FileData>> plist, vector<_tstring> regexStrs) const
{
	shared_ptr<list<FileData>> nlist = make_shared<list<FileData>>();
	vector<_tregex> regs;
	for (const _tstring& str : regexStrs) {
		regs.push_back(_tregex(str));
	}
	for (const FileData& data : *plist) {
		if (!(data.attributes & FILE_ATTRIBUTE_DIRECTORY)) {
			for (const _tregex& reg : regs) {
				if (regex_match(data.name, reg))
					nlist->push_back(data);
			}
		}
	}
	return nlist;
}