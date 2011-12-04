#include "ccidatasorter.h"
#include "SISDefines.h"


CCIdatasorter::CCIdatasorter()
{
}

int CCIdatasorter::Analyze(){
    SISdata* rawdataArray;//, sorteddata;
    int nEvents;
    pair<unsigned int, SISdata*> pData = ReadData<SISdata>("CCI2DataSplitter","D1");
    nEvents = pData.first;
    rawdataArray = pData.second;
    //sorteddata = new SISdata[nEvents];

    quicksort(rawdataArray,0,nEvents-1);

    PostData(nEvents,"sorteddata",rawdataArray);
    return 0;
}

void CCIdatasorter::quicksort(SISdata arr[], int left, int right) {
      int i = left, j = right;
      SISdata tmp;
      int pivot = arr[(left + right) / 2].timestamp;

      /* partition */
      while (i <= j) {
            while (arr[i].timestamp < pivot)
                  i++;
            while (arr[j].timestamp > pivot)
                  j--;
            if (i <= j) {
                  tmp = arr[i];
                  arr[i] = arr[j];
                  arr[j] = tmp;
                  i++;
                  j--;
            }
      };

      /* recursion */
      if (left < j)
            quicksort(arr, left, j);
      if (i < right)
            quicksort(arr, i, right);
}
