class LCDDisplay
{
    // ...
public:
    LCDDisplay();
    BOOL IsVisible();
    UINT GetMaxX();
    UINT GetMaxY();
    void ClearScreen();
    void FillRegion(UINT  X, UINT  Y, UINT  W, UINT  H, Color C);
    void Plot(UINT  X, UINT  Y, Color C);
    void HLine(UINT  X, UINT  Y, UINT  Length, Color C);
    void VLine(UINT  X, UINT  Y, UINT  Length, Color C);
    void Line(UINT X1, UINT Y1, UINT X2, UINT Y2, Color C);
    void TextOut(UINT X, UINT Y, Font Fnt, TextColor TC,
                 PSTR szText);
    void ByteBlt(UINT Offset, UINT Count, BYTE *Buf);
    friend void NextLCDDisplay();
    friend void FirstLCDDisplay();
    friend UINT LCDDisplayCount();
};

// These functions are low-level assembler routines defined
// elsewhere
extern void LCDWriteData(BYTE D);
extern void LCDWriteCommand(BYTE C);

void LCDDisplay::Plot(UINT  X, UINT  Y, Color C)
{
    WORD Address;
    BYTE BitSetCommand;

    if ((X > MaxX) || (Y > MaxY))
        IgnorableError(LCDDisplay_PlotOutOfBounds);
    else
    {
        Address = (30*Y+X/8)+GraphicsHomeAddress;
  
        // Now that we know what byte contains the specified dot,
        // set the LCD display address pointer to point to that
        // location
        LCDWriteData(LOBYTE(Address));
        LCDWriteData(HIBYTE(Address));
        LCDWriteCommand(0x24); // address pointer set command
  
        // Set or clear the correct bit at that address.
        // Figure out which bit in that byte to set.
        BitSetCommand = 7 - (X % 8);
        // Set the color of the bit.
        if (C == Black)
            BitSetCommand = BitSetCommand | 0x08; // Set the bit
        // Finish the command
        BitSetCommand = BitSetCommand | 0xF0; 
        LCDWriteCommand(BitSetCommand);
    }
}

//End of File
