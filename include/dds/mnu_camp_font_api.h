#ifndef DDS_MNU_CAMP_FONT_API_H

#define DDS_MNU_CAMP_FONT_API_H
struct dds_evtViewerInfo_t;

/* Move the viewer's completed glyph into the current draw-buffer queue. */
void mnuCampLinkFontGlyph(struct dds_evtViewerInfo_t *scene);

#endif /* DDS_MNU_CAMP_FONT_API_H */
