#ifndef DDS_BTL_SOUND_NODE_API_H

#define DDS_BTL_SOUND_NODE_API_H
struct ActiveSoundNode;

struct SoundResourceNode;

/* Allocate the two independently linked sound-node kinds. */
struct SoundResourceNode *sndAllocResourceNode(void);

struct ActiveSoundNode *sndAllocListNode(void);

void sndFreeResourceNode(struct SoundResourceNode *node);

void sndFreeListNode(struct ActiveSoundNode *node);

#endif /* DDS_BTL_SOUND_NODE_API_H */
