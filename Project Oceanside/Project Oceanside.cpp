#include <time.h>
#include <chrono>
#include <iostream>

#include "./Constants.h"
#include "./Heap.h"
#include "./Scene.h"

int main()
{
	Scene* scene = new Scene(OOT_3D, "./scenesOoT3D/Lost Woods.json");

	Heap* heap = new Heap(scene, OOT_3D, OOT3D_START, OOT3D_LINK_SIZE);

	heap->SolveOoT3D_LostWoods();

	delete(scene);
	delete(heap);

	return 0;
}
