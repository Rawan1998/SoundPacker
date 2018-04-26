#include "stdafx.h"
#include "SoundPackingLib.h"
#include "HelperFunctions.h"

using namespace std;
using namespace System;
using namespace System::Diagnostics;

const bool operator>(const Folder &a, const Folder &b) {
	return a.totalDuration > b.totalDuration;
}

bool compareFunction(File a, File b) { 
	return a.duration < b.duration; 
}

vector<Folder> WorstFitLS(vector<File> files, int maxDuration) {
	vector<Folder>folders;

	
	for (int i = 0; i < files.size(); i++) {
		int index = -1;
		int Max = -1;
		for (int j = 0; j < folders.size(); j++) {
			if (maxDuration - folders[j].totalDuration >= files[i].duration) {
				if (maxDuration - folders[j].totalDuration>Max) {
					Max = maxDuration - folders[j].totalDuration;
					index = j;
				}
			}
		}
		if (index == -1) {
			folders.push_back(Folder());
			folders[folders.size() - 1].files.push_back(files[i]);
			folders[folders.size() - 1].totalDuration += files[i].duration;
		} else {
			folders[index].files.push_back(files[i]);
			folders[index].totalDuration += files[i].duration;
		}
	}

	return folders;
}

vector<Folder> WorstFitPQ(vector<File> files, int maxDuration) {
	std::priority_queue<Folder, vector<Folder>, greater<Folder> >folders;

	for (int i = 0; i < files.size(); i++) {
		if (folders.empty()) {
			folders.push(Folder());
			Folder temp = folders.top();
			folders.pop();
			temp.files.push_back(files[i]);
			temp.totalDuration += files[i].duration;
			folders.push(temp);
		}

		else if (maxDuration - folders.top().totalDuration >= files[i].duration) {
			Folder temp = folders.top();
			folders.pop();
			temp.files.push_back(files[i]);
			temp.totalDuration += files[i].duration;
			folders.push(temp);
		} else {
			folders.push(Folder());
			Folder temp = folders.top();
			folders.pop();
			temp.files.push_back(files[i]);
			temp.totalDuration += files[i].duration;
			folders.push(temp);
		}
	}

	vector<Folder>folder;
	while (!folders.empty()) {
		folder.push_back(folders.top());
		folders.pop();
	}

	return folder;
}

vector<Folder> worstFitDecreasingLS(vector<File> inputFiles, int maxDuration) {
	sort(inputFiles.rbegin(), inputFiles.rend(), compareFunction);
	vector<Folder>folders;
	for (int i = 0; i < inputFiles.size(); i++) {
		int index = -1;
		int Max = -1;
		for (int j = 0; j < folders.size(); j++) {
			if (maxDuration - folders[j].totalDuration >= inputFiles[i].duration) {
				if (maxDuration - folders[j].totalDuration>Max) {
					Max = maxDuration - folders[j].totalDuration;
					index = j;
				}
			}
		}
		if (index == -1) {
			folders.push_back(Folder());
			folders[folders.size() - 1].files.push_back(inputFiles[i]);
			folders[folders.size() - 1].totalDuration += inputFiles[i].duration;
		}
		else {
			folders[index].files.push_back(inputFiles[i]);
			folders[index].totalDuration += inputFiles[i].duration;
		}
	}

	return folders;
}

vector<Folder> worstFitDecreasingPQ(vector<File> inputFiles, int maxDuration) {
	priority_queue < Folder, vector<Folder>, greater<Folder> > myFolderspq;
	sort(inputFiles.rbegin(), inputFiles.rend(), compareFunction); // O(N Log N)
	Folder fisrtFolder;
	fisrtFolder.totalDuration = 0;
	myFolderspq.push(fisrtFolder); // O(Log M)
	for (int i = 0; i < inputFiles.size(); i++) {
		Folder temp;
		if (inputFiles[i].duration <= (maxDuration - myFolderspq.top().totalDuration)) {
			temp = myFolderspq.top(); //O(1)
			myFolderspq.pop(); // O(log M)
			temp.files.push_back(inputFiles[i]); // O(1)
			temp.totalDuration += inputFiles[i].duration; // O(1)
			myFolderspq.push(temp); // O(Log M)
		}
		else {
			temp.totalDuration = inputFiles[i].duration;
			temp.files.push_back(inputFiles[i]); // O(1)
			myFolderspq.push(temp); //O(Log M)
		}
	}
	vector<Folder> myFolders;
	while (!myFolderspq.empty()) {
		myFolders.push_back(myFolderspq.top()); // O(1)
		myFolderspq.pop(); // O(log M)
	}
	return myFolders;
}


vector<Folder> firstFitDecreasingLS(vector<File> inputFiles, int maxDuration) {
	vector<Folder> myFolders;
	sort(inputFiles.rbegin(), inputFiles.rend(), compareFunction); // O(N Log N)
	for (int i = 0; i < inputFiles.size(); i++) {
		int k = 0;
		for (int j = 0; j < myFolders.size(); j++) {
			if ((maxDuration - myFolders[j].totalDuration) >= inputFiles[i].duration) {
				myFolders[j].files.push_back(inputFiles[i]);
				myFolders[j].totalDuration += inputFiles[i].duration;
				k = 1;
				break;
			}
		}
		if (!k) {
			Folder folder;
			folder.files.push_back(inputFiles[i]);
			folder.totalDuration = inputFiles[i].duration;
			myFolders.push_back(folder);
		}
	}
	return myFolders;
}
vector<Folder> bestFit(vector<File>files, int maxDuration) {

	vector<Folder>folders;

	sort(files.rbegin(), files.rend(), compareFunction); // O(N Log N)

	for (int i = 0; i<files.size(); i++) {		//O(N*N)
		int bestDurationInd = -1;			//O(1)
		for (int j = 0; j<folders.size(); j++) {	//O(N)

			int curDuration = maxDuration - folders[j].totalDuration;	//O(1)

			if (files[i].duration <= curDuration) {	//O(1)

				if (bestDurationInd == -1) {	//O(1)
					bestDurationInd = j;		//O(1)
				}
				else {
					if (folders[j].totalDuration < maxDuration - folders[bestDurationInd].totalDuration) //O(1)
					{	
						bestDurationInd = j;		//O(1)
					}
				}
			}
		}
		if (bestDurationInd == -1) {		//O(1)
			Folder folder;					//O(1)
			folder.totalDuration = 0;		//O(1)
			folder.totalDuration += files[i].duration;		//O(1)
			folder.files.push_back(files[i]);			//O(1)
			folders.push_back(folder);		//O(1)
		}	
		else {
			folders[bestDurationInd].totalDuration += files[i].duration;	//O(1)
			folders[bestDurationInd].files.push_back(files[i]);		//O(1)

		}

	}
	return folders;		//O(1)
}
int dp[10001][351];
bool store[10001][351];
vector<Folder> folderFilling(vector<File> files, int maxDuration) {

	vector<Folder> folders;		//O(1)
	int filesTaken = 0;			//O(1)
	while (filesTaken<files.size()) {		//O(n*n*d)

		memset(dp, -1, sizeof dp);			//O(n)
		memset(store, 0, sizeof store);		//O(n)

		Folder folder;					//O(1)

		for (int i = 0; i <= files.size(); i++) {		//O(n*d)
			for (int j = 0; j <= maxDuration; j++) {	//O(d)
				int duration = 0;		//O(1)
				if (!i)					//O(1)
				{
					dp[i][j] = 0;		//O(1)
				}
				else {
					duration = files[i - 1].duration;	//O(1)

					if (duration <= j && dp[i - 1][j] < duration + dp[i - 1][j - duration] && duration != -1)	//O(1)
					{
						dp[i][j] = duration + dp[i - 1][j - duration];		//O(1)
						store[i][j] = true;									//O(1)
					}
					else {
						dp[i][j] = dp[i - 1][j];				//O(1)
						store[i][j] = false;					//O(1)
					}
				}
			}
		}	
		int j = maxDuration;		//O(1)
		for (int i = files.size(); i >= 1; i--) {		//O(n)
			if (store[i][j]) {							//O(1)
				folder.totalDuration += files[i - 1].duration;		//O(1)
				folder.files.push_back(files[i - 1]);	//O(1)	
				j -= files[i - 1].duration;			//O(1)
				files[i - 1].duration = -1;			//O(1)
				filesTaken++;			//O(1)
			}
		}
		folders.push_back(folder);			//O(1)
	}
	return folders;			//O(1)
}