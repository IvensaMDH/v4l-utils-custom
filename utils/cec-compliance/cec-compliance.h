/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * CEC API compliance test tool.
 *
 * Copyright 2016 Cisco Systems, Inc. and/or its affiliates. All rights reserved.
 */

#ifndef _CEC_COMPLIANCE_H_
#define _CEC_COMPLIANCE_H_

#include <linux/cec-funcs.h>
#include "cec-htng-funcs.h"

#include <cec-info.h>

#include <vector>

#define TAG_AUDIO_RATE_CONTROL		1
#define TAG_ARC_CONTROL 		(1 << 1)
#define TAG_CAP_DISCOVERY_CONTROL 	(1 << 2)
#define TAG_DECK_CONTROL 		(1 << 3)
#define TAG_DEVICE_MENU_CONTROL 	(1 << 4)
#define TAG_DEVICE_OSD_TRANSFER 	(1 << 5)
#define TAG_DYNAMIC_AUTO_LIPSYNC 	(1 << 6)
#define TAG_OSD_DISPLAY 		(1 << 7)
#define TAG_ONE_TOUCH_PLAY 		(1 << 8)
#define TAG_ONE_TOUCH_RECORD 		(1 << 9)
#define TAG_POWER_STATUS 		(1 << 10)
#define TAG_REMOTE_CONTROL_PASSTHROUGH	(1 << 11)
#define TAG_ROUTING_CONTROL 		(1 << 12)
#define TAG_SYSTEM_AUDIO_CONTROL 	(1 << 13)
#define TAG_SYSTEM_INFORMATION		(1 << 14)
#define TAG_TIMER_PROGRAMMING 		(1 << 15)
#define TAG_TUNER_CONTROL 		(1 << 16)
#define TAG_VENDOR_SPECIFIC_COMMANDS 	(1 << 17)
#define TAG_STANDBY_RESUME		(1 << 18)

#define TAG_INTERACTIVE		(1 << 19)
#define TAG_CORE 0
#define TAG_ALL (~TAG_INTERACTIVE)

#define NUM_TAGS 21

struct short_audio_desc {
	/* Byte 1 */
	__u8 num_channels;
	__u8 format_code;

	/* Byte 2 */
	__u8 sample_freq_mask;

	/* Byte 3 */
	union {
		__u8 bit_depth_mask;    // LPCM
		__u8 max_bitrate;       // Format codes 2-8
		__u8 format_dependent;  // Format codes 9-13
		__u8 wma_profile;       // WMA Pro
		__u8 frame_length_mask; // Extension type codes 4-6, 8-10
	};
	__u8 mps;                       // Format codes 8-10
	__u8 extension_type_code;
};

#define SAD_FMT_CODE_LPCM 		1
#define SAD_FMT_CODE_AC3		2
#define SAD_FMT_CODE_MPEG1		3
#define SAD_FMT_CODE_MP3 		4
#define SAD_FMT_CODE_MPEG2 		5
#define SAD_FMT_CODE_AAC_LC 		6
#define SAD_FMT_CODE_DTS 		7
#define SAD_FMT_CODE_ATRAC 		8
#define SAD_FMT_CODE_ONE_BIT_AUDIO	9
#define SAD_FMT_CODE_ENHANCED_AC3	10
#define SAD_FMT_CODE_DTS_HD 		11
#define SAD_FMT_CODE_MAT 		12
#define SAD_FMT_CODE_DST 		13
#define SAD_FMT_CODE_WMA_PRO 		14
#define SAD_FMT_CODE_EXTENDED 		15

#define SAD_BIT_DEPTH_MASK_16 		1
#define SAD_BIT_DEPTH_MASK_20 		(1 << 1)
#define SAD_BIT_DEPTH_MASK_24 		(1 << 2)

#define SAD_SAMPLE_FREQ_MASK_32 	1
#define SAD_SAMPLE_FREQ_MASK_44_1 	(1 << 1)
#define SAD_SAMPLE_FREQ_MASK_48 	(1 << 2)
#define SAD_SAMPLE_FREQ_MASK_88_2 	(1 << 3)
#define SAD_SAMPLE_FREQ_MASK_96 	(1 << 4)
#define SAD_SAMPLE_FREQ_MASK_176_4 	(1 << 5)
#define SAD_SAMPLE_FREQ_MASK_192 	(1 << 6)

#define SAD_FRAME_LENGTH_MASK_960 	1
#define SAD_FRAME_LENGTH_MASK_1024	(1 << 1)

#define SAD_EXT_TYPE_MPEG4_HE_AAC 		4
#define SAD_EXT_TYPE_MPEG4_HE_AACv2 		5
#define SAD_EXT_TYPE_MPEG4_AAC_LC 		6
#define SAD_EXT_TYPE_DRA 			7
#define SAD_EXT_TYPE_MPEG4_HE_AAC_SURROUND 	8
#define SAD_EXT_TYPE_MPEG4_AAC_LC_SURROUND	10
#define SAD_EXT_TYPE_MPEG_H_3D_AUDIO		11
#define SAD_EXT_TYPE_AC_4			12
#define SAD_EXT_TYPE_LPCM_3D_AUDIO		13

extern bool show_info;
extern bool show_colors;
extern bool show_warnings;
extern bool exit_on_fail;
extern bool exit_on_warn;
extern unsigned warnings;
extern unsigned reply_threshold;
extern time_t long_timeout;

struct remote {
	bool recognized_op[256];
	bool unrecognized_op[256];
	unsigned prim_type;
	__u16 phys_addr;
	char osd_name[15];
	char language[4];
	__u8 cec_version;
	__u8 rc_profile;
	__u8 dev_features;
	__u8 all_device_types;
	__u32 vendor_id;
	bool has_osd;
	bool has_power_status;
	bool has_image_view_on;
	bool has_text_view_on;
	bool in_standby;
	bool has_remote_control_passthrough;
	bool source_has_arc_rx;
	bool sink_has_arc_tx;
	bool arc_initiated;
	bool has_sys_audio_mode_req;
	bool has_set_sys_audio_mode;
	bool has_sad;
	__u8 supp_format_id;
	__u8 supp_format_code;
	__u8 volume;
	__u8 mute;
	bool has_aud_rate;
	bool has_deck_ctl;
	bool has_analogue_timer;
	__u8 bcast_sys;
	__u8 dig_bcast_sys;
	bool has_rec_tv;
	bool has_cdc;
};

struct node {
	int fd;
	const char *device;
	bool is_vivid;
	bool has_cec20;
	unsigned caps;
	unsigned available_log_addrs;
	unsigned num_log_addrs;
	unsigned adap_la_mask;
	__u8 log_addr[CEC_MAX_LOG_ADDRS];
	__u32 msg_fl_mask;
	unsigned remote_la_mask;
	struct remote remote[16];
	__u16 phys_addr;
	__u32 vendor_id;
	bool in_standby;
	__u8 prim_devtype;
	time_t current_time;
};

struct remote_subtest {
	const char *name;
	const __u16 la_mask;
	int (*const test_fn)(struct node *node, unsigned me, unsigned la, bool interactive);
	bool in_standby;
	bool for_cec20;
};

using vec_remote_subtests = std::vector<remote_subtest>;

#ifndef __FILE_NAME__
#define __FILE_NAME__ __FILE__
#endif

#define OK			0
#define FAIL			1
#define OK_PRESUMED		2
#define OK_NOT_SUPPORTED	3
#define OK_REFUSED		4
#define OK_UNEXPECTED		5
#define OK_EXPECTED_FAIL	6
#define NOTAPPLICABLE		7
#define FAIL_CRITICAL		8
#define DONT_CARE		255

#define CEC_LOG_ADDR_MASK_ALL 0xffff

#define COLOR_GREEN(s) "\033[32m" s "\033[0m"
#define COLOR_RED(s) "\033[1;31m" s "\033[0m"
#define COLOR_BOLD(s) "\033[1m" s "\033[0m"

#define info(fmt, args...) 						\
	do {								\
		if (show_info) {					\
			printf("\t\tinfo: " fmt, ##args);		\
			fflush(stdout);					\
		}							\
	} while (0)

#define announce(fmt, args...) 						\
	do {								\
		printf("\t\t>>> " fmt "\n", ##args);			\
		fflush(stdout);						\
	} while (0)

#define interactive_info(block, fmt, args...)				\
	do {								\
		if (interactive) {					\
			printf("\t\t>>> " fmt "\n", ##args);		\
			if (block) {					\
				printf("\t\t>>> Press ENTER to proceed.\n"); \
				getchar();				\
			}						\
		}							\
	} while(0)

#define warn(fmt, args...) 						\
({									\
	warnings++;							\
	if (show_warnings) {						\
		printf("\t\%s: %s(%d): " fmt,				\
		       show_colors ? COLOR_BOLD("warn") : "warn",	\
		       __FILE_NAME__, __LINE__, ##args);		\
		fflush(stdout);						\
	}								\
	if (exit_on_warn)						\
		std::exit(EXIT_FAILURE);				\
	0;								\
})

#define warn_once(fmt, args...)						\
	do {								\
		static bool show;					\
									\
		if (!show) {						\
			show = true;					\
			warn(fmt, ##args);				\
		}							\
	} while (0)

#define warn_on_test(test) 						\
	do {								\
		if (test)						\
			warn("%s\n", #test);				\
	} while (0)

#define warn_once_on_test(test) 					\
	do {								\
		if (test)						\
			warn_once("%s\n", #test);			\
	} while (0)

#define fail(fmt, args...) 						\
({ 									\
	printf("\t\t%s: %s(%d): " fmt, show_colors ?			\
	       COLOR_RED("fail") : "fail", __FILE_NAME__, __LINE__, ##args);	\
	fflush(stdout);							\
	if (exit_on_fail)						\
		std::exit(EXIT_FAILURE);				\
	FAIL;								\
})

#define fail_once(fmt, args...)						\
	do {								\
		static bool show;					\
									\
		if (!show) {						\
			show = true;					\
			fail(fmt, ##args);				\
		}							\
	} while (0)

#define fail_or_warn(node, fmt, args...)				\
({									\
 	if ((node)->in_standby)						\
		warn(fmt, ##args);					\
	else								\
		fail(fmt, ##args);					\
	(node)->in_standby ? 0 : FAIL;					\
})

#define fail_on_test(test) 						\
	do {								\
		if (test)						\
			return fail("%s\n", #test);			\
	} while (0)

#define fail_on_test_v2(version, test) fail_on_test(version >= CEC_OP_CEC_VERSION_2_0 && (test))

#define fail_on_test_v2_warn(version, test)				\
	do {								\
		if (test) {						\
			if (version >= CEC_OP_CEC_VERSION_2_0)		\
				return fail("%s\n", #test);		\
			else						\
				warn("fails in CEC 2.0: %s\n", #test);	\
		}							\
	} while(0)

static inline char get_yn()
{
	char c;

	while ((c = tolower(getchar())) != 'y' && c != 'n');
	getchar();
	return c;
}

static inline bool question(const char* prompt)
{
	printf("\t\t>>> %s (y/n) ", prompt);
	return get_yn() == 'y';
}

int cec_named_ioctl(struct node *node, const char *name,
		    unsigned long int request, void *parm);

#define doioctl(n, r, p) cec_named_ioctl(n, #r, r, p)

const char *opcode2s(__u8 opcode);
std::string opcode2s(const struct cec_msg *msg);

static inline bool is_tv(unsigned la, unsigned prim_type)
{
	return cec_has_tv(1 << la) ||
		(cec_has_specific(1 << la) && prim_type == CEC_OP_PRIM_DEVTYPE_TV);
}

static inline bool is_playback_or_rec(unsigned la)
{
	return cec_has_playback(1 << la) || cec_has_record(1 << la);
}

static inline bool cec_msg_status_is_abort(const struct cec_msg *msg)
{
	return msg->rx_status & CEC_RX_STATUS_FEATURE_ABORT;
}

static inline __u8 abort_reason(const struct cec_msg *msg)
{
	return msg->msg[3];
}

static inline bool unrecognized_op(const struct cec_msg *msg)
{
	if (!cec_msg_status_is_abort(msg))
		return false;
	if (abort_reason(msg) == CEC_OP_ABORT_UNRECOGNIZED_OP)
		return true;
	if (abort_reason(msg) == CEC_OP_ABORT_UNDETERMINED) {
		warn("Opcode %x was undetermined and is treated as not supported.\n", msg->msg[2]);
		return true;
	}
	return false;
}

static inline bool refused(const struct cec_msg *msg)
{
	return cec_msg_status_is_abort(msg) && abort_reason(msg) == CEC_OP_ABORT_REFUSED;
}

static inline bool incorrect_mode(const struct cec_msg *msg)
{
	return cec_msg_status_is_abort(msg) && abort_reason(msg) == CEC_OP_ABORT_INCORRECT_MODE;
}

static inline bool timed_out(const struct cec_msg *msg)
{
	return msg->rx_status & CEC_RX_STATUS_TIMEOUT;
}

static inline bool timed_out_or_abort(const struct cec_msg *msg)
{
	return timed_out(msg) || cec_msg_status_is_abort(msg);
}

static inline unsigned response_time_ms(const struct cec_msg *msg)
{
	unsigned ms = (msg->rx_ts - msg->tx_ts) / 1000000;

	// Compensate for the time it took (approx.) to receive the
	// message.
	if (ms >= msg->len * 24)
		return ms - msg->len * 24;
	return 0;
}

bool transmit_timeout(struct node *node, struct cec_msg *msg,
		      unsigned timeout = 2000);

static inline bool transmit(struct node *node, struct cec_msg *msg)
{
	return transmit_timeout(node, msg, 0);
}

static inline void mode_set_follower(struct node *node)
{
	__u32 mode = CEC_MODE_INITIATOR | CEC_MODE_FOLLOWER;

	doioctl(node, CEC_S_MODE, &mode);
}

static inline void mode_set_initiator(struct node *node)
{
	__u32 mode = CEC_MODE_INITIATOR;

	doioctl(node, CEC_S_MODE, &mode);
}

static inline unsigned get_ts_ms()
{
	struct timespec timespec;

	clock_gettime(CLOCK_MONOTONIC, &timespec);
	return timespec.tv_sec * 1000ull + timespec.tv_nsec / 1000000;
}

const char *result_name(int res, bool show_colors);
const char *ok(int res);
const char *power_status2s(__u8 power_status);
const char *bcast_system2s(__u8 bcast_system);
const char *dig_bcast_system2s(__u8 bcast_system);
int check_0(const void *p, int len);
int util_receive(struct node *node, unsigned la, unsigned timeout,
		 struct cec_msg *msg, __u8 sent_msg,
		 __u8 reply1, __u8 reply2 = 0);
std::string safename(const char *name);
std::string current_ts();

// CEC adapter tests
void testAdapter(struct node &node, struct cec_log_addrs &laddrs,
		 const char *device);

// CEC fuzzing test
int testFuzzing(struct node &node, unsigned me, unsigned la);

// CEC core tests
int testCore(struct node *node);
int core_unknown(struct node *node, unsigned me, unsigned la, bool interactive);
int core_abort(struct node *node, unsigned me, unsigned la, bool interactive);
int system_info_polling(struct node *node, unsigned me, unsigned la, bool interactive);
int system_info_phys_addr(struct node *node, unsigned me, unsigned la, bool interactive);
int system_info_version(struct node *node, unsigned me, unsigned la, bool interactive);
int system_info_get_menu_lang(struct node *node, unsigned me, unsigned la, bool interactive);
int system_info_give_features(struct node *node, unsigned me, unsigned la, bool interactive);
int vendor_specific_commands_id(struct node *node, unsigned me, unsigned la, bool interactive);
int device_osd_transfer_give(struct node *node, unsigned me, unsigned la, bool interactive);

// CEC processing
int testProcessing(struct node *node, unsigned me);

// CEC testing
void collectTests(void);
void listTests(void);
int setExpectedResult(char *optarg, bool no_warnings);
void testRemote(struct node *node, unsigned me, unsigned la, unsigned test_tags,
			     bool interactive, bool show_ts);

// cec-test-tuner-record-timer.cpp
extern const vec_remote_subtests tuner_ctl_subtests;
extern const vec_remote_subtests one_touch_rec_subtests;
extern const vec_remote_subtests timer_prog_subtests;

// cec-test-audio.cpp
extern const vec_remote_subtests sac_subtests;
extern const vec_remote_subtests dal_subtests;
extern const vec_remote_subtests arc_subtests;
extern const vec_remote_subtests audio_rate_ctl_subtests;

// cec-test-power.cpp
bool util_interactive_ensure_power_state(struct node *node, unsigned me, unsigned la, bool interactive,
					 __u8 target_pwr);
int standby_resume_wakeup(struct node *node, unsigned me, unsigned la, bool interactive);
extern const vec_remote_subtests standby_subtests;
extern const vec_remote_subtests one_touch_play_subtests;
extern const vec_remote_subtests power_status_subtests;
extern const vec_remote_subtests standby_resume_subtests;

#endif
