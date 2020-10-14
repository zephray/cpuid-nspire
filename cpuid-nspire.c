#include <os.h>
#include <SDL/SDL_config.h>
#include <SDL/SDL.h>


#define _ARM_MRC(coproc, opcode1, Rt, CRn, CRm, opcode2) \
    asm volatile ("mrc p" #coproc ", " #opcode1 ", %[output], c" #CRn ", c" #CRm ", " #opcode2 "\n" : [output] "=r" (Rt))
    
#define _ARM_MCR(coproc, opcode1, Rt, CRn, CRm, opcode2) \
    asm volatile ("mcr p" #coproc ", " #opcode1 ", %[input], c" #CRn ", c" #CRm ", " #opcode2 "\n" :: [input] "r" (Rt))
    
const char *get_impl_name(uint8_t impl) {
    if (impl == 0x41)
        return "ARM";
    else if (impl == 0x44)
        return "DEC";
    else if (impl == 0x66)
        return "Faraday";
    else if (impl == 0x69)
        return "Intel";
    else
        return "Unknown";
}

const char *get_arch_name(uint8_t arch) {
    if (arch == 0x1)
        return "ARMv4";
    else if (arch == 0x2)
        return "ARMv4T";
    else if (arch == 0x3)
        return "ARMv5";
    else if (arch == 0x4)
        return "ARMv5T";
    else if (arch == 0x5)
        return "ARMv5TE";
    else if (arch == 0x6)
        return "ARMv5TEJ";
    else if (arch == 0x7)
        return "ARMv6";
    else
        return "Unknown";
}

const char *get_ctype_name(uint8_t ctype) {
    if (ctype == 0xe)
        return "Write-back";
    else
        return "Unknown";
}

const char *get_cache_size_name(uint8_t size) {
    if (size == 0x3)
        return "4 KB";
    else if (size == 0x4)
        return "8 KB";
    else if (size == 0x5)
        return "16 KB";
    else if (size == 0x6)
        return "32 KB";
    else if (size == 0x7)
        return "64 KB";
    else if (size == 0x8)
        return "128 KB";
    else
        return "Unknown";
}

const char *get_cache_assoc_name(uint8_t assoc) {
    if (assoc == 0x2)
        return "4-way";
    else
        return "Unknown";
}

const char *get_cache_len_name(uint8_t len) {
    if (len == 0x2)
        return "32 bytes";
    else
        return "Unknown";
}

int main(void) {
    SDL_Surface *screen;
    nSDL_Font *font;
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(320, 240, has_colors ? 16 : 8, SDL_SWSURFACE);
    font = nSDL_LoadFont(NSDL_FONT_TINYTYPE, 0, 0, 0);
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
    nSDL_DrawString(screen, font, 0, 0, "CPUID for nspire");

    // CPUID
    uint32_t cpuid;
    _ARM_MRC(15, 0, cpuid, 0, 0, 0);

    uint8_t impl = cpuid >> 24;
    uint8_t vari = (cpuid >> 20) & 0xf;
    uint8_t arch = (cpuid >> 16) & 0xf;
    uint16_t part = (cpuid >> 4) & 0xfff;
    uint8_t rev = cpuid & 0xf;

    nSDL_DrawString(screen, font, 0, 16, "Core");

    char str[64];
    snprintf(str, 64, "Implementer: %s (0x%02x)", get_impl_name(impl), impl);
    nSDL_DrawString(screen, font, 8, 24, str);

    snprintf(str, 64, "Architecture: %s (0x%01x)", get_arch_name(arch), arch);
    nSDL_DrawString(screen, font, 8, 32, str);

    snprintf(str, 64, "Part Number: 0x%03x", part);
    nSDL_DrawString(screen, font, 8, 40, str);

    snprintf(str, 64, "Variant: 0x%01x", vari);
    nSDL_DrawString(screen, font, 8, 48, str);

    snprintf(str, 64, "Revision: 0x%01x", rev);
    nSDL_DrawString(screen, font, 8, 56, str);

    // Cache
    uint32_t cacheid;
    _ARM_MRC(15, 0, cacheid, 0, 0, 1);

    nSDL_DrawString(screen, font, 0, 72, "L1 Cache");

    uint8_t ctype = (cacheid >> 25) & 0xf;
    uint8_t sbit = (cacheid >> 24) & 0x1;
    uint16_t dsize = (cacheid >> 12) & 0xfff;
    uint16_t isize = (cacheid) & 0xfff;
    snprintf(str, 64, "Type: %s (0x%01x)", get_ctype_name(ctype), ctype);
    nSDL_DrawString(screen, font, 8, 80, str);

    snprintf(str, 64, "Unified: %s", (sbit) ? "No" : "Yes");
    nSDL_DrawString(screen, font, 8, 88, str);

    uint8_t csize = (dsize >> 6) & 0xf;
    snprintf(str, 64, "D-Cache Size: %s (0x%01x)", get_cache_size_name(csize), csize);
    nSDL_DrawString(screen, font, 8, 96, str);

    uint8_t assoc = (dsize >> 3) & 0x7;
    snprintf(str, 64, "D-Cache Associtivity: %s (0x%01x)", get_cache_assoc_name(assoc), assoc);
    nSDL_DrawString(screen, font, 8, 104, str);

    uint8_t len = (dsize) & 0x3;
    snprintf(str, 64, "D-Cache Line Lnegth: %s (0x%01x)", get_cache_len_name(len), len);
    nSDL_DrawString(screen, font, 8, 112, str);

    csize = (isize >> 6) & 0xf;
    snprintf(str, 64, "I-Cache Size: %s (0x%01x)", get_cache_size_name(csize), csize);
    nSDL_DrawString(screen, font, 8, 120, str);

    assoc = (isize >> 3) & 0x7;
    snprintf(str, 64, "I-Cache Associtivity: %s (0x%01x)", get_cache_assoc_name(assoc), assoc);
    nSDL_DrawString(screen, font, 8, 128, str);

    len = (isize) & 0x3;
    snprintf(str, 64, "I-Cache Line Lnegth: %s (0x%01x)", get_cache_len_name(len), len);
    nSDL_DrawString(screen, font, 8, 136, str);

    SDL_Flip(screen);

    int quit = 0;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                quit = 1;
            }
        }
    }

    SDL_Quit();
    return 0;
}
