#ifndef DDS_DDS3_OWNED_NODE_H

#define DDS_DDS3_OWNED_NODE_H
#include "common.h"

struct _FRQ;

typedef struct Dds3IntrusiveNode Dds3IntrusiveNode;

typedef struct Dds3IntrusiveNodeCallbacks Dds3IntrusiveNodeCallbacks;

/* Shared prefix used by the font node and by L2D rectangles. */
struct Dds3IntrusiveNode {
    Dds3IntrusiveNode *prev;
    Dds3IntrusiveNode *next;
    s32 value;
    /* A registered node borrows this persistent callback table for dispatch. */
    const Dds3IntrusiveNodeCallbacks *vtable;
};

/* The list object contains the head and tail words at its native address. */
typedef struct Dds3IntrusiveNodeList {
    Dds3IntrusiveNode *head;
    Dds3IntrusiveNode *tail;
} Dds3IntrusiveNodeList;

/* Callback tables are shared by owners with different payloads. */
struct Dds3IntrusiveNodeCallbacks {
    void (*destroy)(Dds3IntrusiveNode *owner);
    void (*update)(const Dds3IntrusiveNode *owner);
};

/* The font-owned node's payload is specifically a retained glyph pointer. */
typedef struct Dds3FontNode {
    Dds3IntrusiveNode owner;
    struct _FRQ *glyph;
} Dds3FontNode;

/* The L2D callback owner extends the intrusive prefix with a rectangle. */
typedef struct Dds3L2dRectangle {
    Dds3IntrusiveNode owner;
    s32 left;
    s32 top;
    s32 width;
    s32 height;
    s32 depth;
    s32 color;
} Dds3L2dRectangle;

extern Dds3IntrusiveNodeList dds3OwnedNodeListHead;

extern Dds3IntrusiveNodeCallbacks dds3FontNodeVTable;

void dds3SetLinkedNodeValue(Dds3IntrusiveNode *node, u32 value);

Dds3FontNode *dds3CreateFontNode(s32 x, s32 y, const char *text);

typedef char Dds3IntrusiveNodeSizeCheck[sizeof(Dds3IntrusiveNode) == 0x10 ? 1 : -1];

typedef char Dds3IntrusiveNodeListSizeCheck[sizeof(Dds3IntrusiveNodeList) == 0x08 ? 1 : -1];

typedef char Dds3IntrusiveNodeCallbacksSizeCheck[sizeof(Dds3IntrusiveNodeCallbacks) == 0x08 ? 1 : -1];

typedef char Dds3FontNodeSizeCheck[sizeof(Dds3FontNode) == 0x14 ? 1 : -1];

#endif /* DDS_DDS3_OWNED_NODE_H */
