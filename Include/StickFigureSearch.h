#pragma once


void __declspec(dllimport) InitializeStickFigureSearch(const char *bvh_file, const char *joint_map_file);
void __declspec(dllimport) AddStickFigureSearchCB(void (*cb)(int start_frame, int length, void *data), void *data=0);