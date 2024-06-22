/*
 * sunxi_drm_connector.h
 *
 * Copyright (c) 2007-2024 Allwinnertech Co., Ltd.
 * Author: zhengxiaobin <zhengxiaobin@allwinnertech.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef _SUNXI_DRM_CONNECTOR_H
#define _SUNXI_DRM_CONNECTOR_H

#include <edid.h>
#include <linux/list.h>
#include <include.h>
struct sunxi_drm_bridge;
struct sunxi_drm_panel;
struct sunxi_drm_phy;
struct display_state;
struct sunxi_drm_device;
struct drm_display_mode;


struct sunxi_drm_connector {
	struct sunxi_drm_device *drm;
	struct udevice *dev;
	struct sunxi_drm_bridge *bridge;
	struct sunxi_drm_panel *panel;
	struct sunxi_drm_phy *phy;
	struct list_head head;
	int id;
	int type;
	bool hpd;
	const struct sunxi_drm_connector_funcs *funcs;
	void *data;
};


struct sunxi_drm_connector_funcs {
	/*
	 * pre init connector, prepare some parameter out_if, this will be
	 * used by rockchip_display.c and vop
	 */
	int (*pre_init)(struct sunxi_drm_connector *connector, struct display_state *state);

	/*
	 * init connector, prepare resource to ensure
	 * detect and get_timing can works
	 */
	int (*init)(struct sunxi_drm_connector *connector, struct display_state *state);

	void (*deinit)(struct sunxi_drm_connector *connector, struct display_state *state);
	/*
	 * Optional, if connector not support hotplug,
	 * Returns:
	 *   0 means disconnected, else means connected
	 */
	int (*detect)(struct sunxi_drm_connector *connector, struct display_state *state);
	/*
	 * Optional, if implement it, need fill the timing data:
	 *     state->conn_state->mode
	 * you can refer to the rockchip_display: display_get_timing(),
	 * Returns:
	 *   0 means success, else means failed
	 */
	int (*get_timing)(struct sunxi_drm_connector *connector, struct display_state *state);
	/*
	 * Optional, if implement it, need fill the edid data:
	 *     state->conn_state->edid
	 * Returns:
	 *   0 means success, else means failed
	 */
	int (*get_edid)(struct sunxi_drm_connector *connector, struct display_state *state);
	/*
	 * call before crtc enable.
	 */
	int (*prepare)(struct sunxi_drm_connector *connector, struct display_state *state);
	/*
	 * call after crtc enable
	 */
	int (*enable)(struct sunxi_drm_connector *connector, struct display_state *state);
	int (*disable)(struct sunxi_drm_connector *connector, struct display_state *state);
	void (*unprepare)(struct sunxi_drm_connector *connector, struct display_state *state);

	int (*check)(struct sunxi_drm_connector *connector, struct display_state *state);
	int (*mode_valid)(struct sunxi_drm_connector *connector, struct display_state *state);
};



int sunxi_drm_connector_pre_init(struct display_state *state);
int sunxi_drm_connector_pre_enable(struct display_state *state);
int sunxi_drm_connector_enable(struct display_state *state);
int sunxi_drm_connector_disable(struct display_state *state);
int sunxi_drm_connector_post_disable(struct display_state *state);
bool sunxi_drm_connector_detect(struct display_state *state);
int sunxi_drm_connector_bind(struct sunxi_drm_connector *conn, struct udevice *dev, int id,
			    const struct sunxi_drm_connector_funcs *funcs, void *data, int type);

int sunxi_drm_connector_init(struct display_state *state);
int sunxi_drm_connector_get_timing(struct display_state *state);
int sunxi_drm_connector_get_edid(struct display_state *state);
int sunxi_drm_connector_deinit(struct display_state *state);
struct sunxi_drm_connector *
get_sunxi_drm_connector_by_device(struct sunxi_drm_device *drm,
				  struct udevice *dev);
struct sunxi_drm_connector *
get_sunxi_drm_connector_by_type(struct sunxi_drm_device *drm, unsigned int type);

int drm_mode_to_sunxi_video_timings(struct drm_display_mode *mode,
				    struct disp_video_timings *timings);

#endif /*End of file*/
