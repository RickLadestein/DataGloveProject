#ifndef MUXDATA
#define MUXDATA

//The mux's own address types
enum I2CMUXADDR {
    VARIANT_DEFAULT = 0x70,
    VARIANT1 = 0x70,
    VARIANT2 = 0x71,
    VARIANT3 = 0x72,
    VARIANT4 = 0x73,
    VARIANT5 = 0x74,
    VARIANT6 = 0x75,
    VARIANT7 = 0x76
};

//The mux selectable adresses
enum MUXADDR {
    MUX_ADDR_0 = 1 << 0,
    MUX_ADDR_1 = 1 << 1,
    MUX_ADDR_2 = 1 << 2,
    MUX_ADDR_3 = 1 << 3,
    MUX_ADDR_4 = 1 << 4,
    MUX_ADDR_5 = 1 << 5,
    MUX_ADDR_6 = 1 << 6,
    MUX_ADDR_7 = 1 << 7
};
#endif
