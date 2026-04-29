#include "sierrachart.h"

SCDLLName("CBC Flip")

SCSFExport scsf_CBC_Flip(SCStudyInterfaceRef sc)
{
    SCSubgraphRef State = sc.Subgraph[0];   // 0 = Bears, 1 = Bulls
    SCSubgraphRef Bulls = sc.Subgraph[1];
    SCSubgraphRef Bears = sc.Subgraph[2];

    if (sc.SetDefaults)
    {
        sc.GraphName = "CBC Flip";
        sc.AutoLoop = 0;
        sc.UpdateAlways = 1;
        sc.GraphRegion = 0;

        State.Name = "State";
        State.DrawStyle = DRAWSTYLE_IGNORE;

        Bulls.Name = "Long";
        Bulls.DrawStyle = DRAWSTYLE_TRIANGLE_UP;
        Bulls.PrimaryColor = RGB(128,255,128);
        Bulls.LineWidth = 10;
        Bulls.DrawZeros = false;

        Bears.Name = "Short";
        Bears.DrawStyle = DRAWSTYLE_TRIANGLE_DOWN;
        Bears.PrimaryColor = RGB(255,128,128);
        Bears.LineWidth = 10;
        Bears.DrawZeros = false;

        return;
    }

    int start = sc.UpdateStartIndex;

    // Ensure we start at 1 (need prior bar)
    if (start < 1)
        start = 1;

    // Full recalculation reset
    if (sc.UpdateStartIndex == 0)
        State[0] = 0;  // default to Bears

    for (int i = start; i < sc.ArraySize; i++)
    {
        int prevState = (int)State[i - 1];
        int newState = prevState;

        float closePrice = sc.Close[i];
        float priorHigh  = sc.High[i - 1];
        float priorLow   = sc.Low[i - 1];

        // Core logic
        if (prevState == 1 && closePrice < priorLow)
            newState = 0;

        if (prevState == 0 && closePrice > priorHigh)
            newState = 1;

        State[i] = newState;

        bool switchedToBull = (newState == 1 && prevState == 0);
        bool switchedToBear = (newState == 0 && prevState == 1);

        // Clear previous markers
        Bulls[i] = 0;
        Bears[i] = 0;

        if (switchedToBull)
            Bulls[i] = sc.Low[i] - 8 * sc.TickSize;

        if (switchedToBear)
            Bears[i] = sc.High[i] + 8 * sc.TickSize;
    }
}
