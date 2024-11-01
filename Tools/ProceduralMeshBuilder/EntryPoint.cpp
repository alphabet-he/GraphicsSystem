/*
	The main() function is where the program starts execution
*/

// Includes
//=========

#include "cProceduralMeshBuilder.h"

// Entry Point
//============

int main(int i_argumentCount, char** i_arguments)
{
	return eae6320::Assets::Build<eae6320::Assets::cProceduralMeshBuilder>(i_arguments, i_argumentCount);
}
