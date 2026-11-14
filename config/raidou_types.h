/* Types whose Raidou layout differs from the SMT3 or DDS type of the same name. tools/raidou_c.py
 * port puts these definitions in place of the source's in the declaration blocks it writes. */

/* SMT3's has a ninth word (data); Raidou's frFontSystem_t entries are 0x20 apart. */
struct frFontData_t {
    sdfMemHandle_t *allocation;
    frFontHeader_t *resourceHeader;
    s32 metricByteCount;
    s32 valueByteCount;
    s8 *prop;
    void *ysht;
    GsFont_t **trans;
    struct frFontResource_t *resource;
};
