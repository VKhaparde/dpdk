/*-
 *   BSD LICENSE
 *
 *   Copyright(c) Broadcom Limited.
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *     * Neither the name of Broadcom Corporation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _BNXT_CPR_H_
#define _BNXT_CPR_H_
#include <stdbool.h>

#define CMP_VALID(cons, v, cp_ring)					\
	(!!(((struct cmpl_base *)(&cp_ring->cp_desc_ring[cons]))->info3_v & CMPL_BASE_V) == !(v))

#define CMP_TYPE(cmp)						\
	(((struct cmpl_base *)cmp)->type & CMPL_BASE_TYPE_MASK)

#define NEXT_CMP_NB(cpr, idx, v)	do { \
	(idx)++; \
	v ^= !!(idx & (cpr)->cp_ring_struct->ring_size); \
	idx &= (cpr)->cp_ring_struct->ring_mask; \
} while(0)

#define NEXT_CMP(cpr, idx, v)	do { \
	(idx)++; \
	if (unlikely((idx) == (cpr)->cp_ring_struct->ring_size)) { \
		(v) = !(v); \
		idx = 0; \
	} \
} while(0)

#define DB_CP_REARM_FLAGS	(DB_KEY_CP | DB_IDX_VALID)
#define DB_CP_FLAGS		(DB_KEY_CP | DB_IDX_VALID | DB_IRQ_DIS)

#define B_CP_DB_ARM(cpr)	(*(uint32_t *)((cpr)->cp_doorbell) = DB_KEY_CP)
#define B_CP_DB_DISARM(cpr)	(*(uint32_t *)((cpr)->cp_doorbell) = DB_KEY_CP | DB_IRQ_DIS)

#define B_CP_DB_IDX_ARM(cpr, cons)					\
		(*(uint32_t *)((cpr)->cp_doorbell) = (DB_CP_REARM_FLAGS | \
				cons))

#define B_CP_DB_IDX_DISARM(cpr, cons)					\
		rte_smp_wmb();						\
		(*(uint32_t *)((cpr)->cp_doorbell) = (DB_CP_FLAGS |	\
				cons))

struct bnxt_ring;
struct bnxt_cp_ring_info {
	void			*cp_doorbell;

	struct cmpl_base	*cp_desc_ring;

	phys_addr_t		cp_desc_mapping;

	struct ctx_hw_stats	*hw_stats;
	phys_addr_t		hw_stats_map;
	uint32_t		hw_stats_ctx_id;

	struct bnxt_ring	*cp_ring_struct;
	uint16_t		cp_cons;
	bool			v;
};

#define RX_CMP_L2_ERRORS						\
	(RX_PKT_CMPL_ERRORS_BUFFER_ERROR_MASK | RX_PKT_CMPL_ERRORS_CRC_ERROR)


struct bnxt;
int bnxt_alloc_def_cp_ring(struct bnxt *bp);
void bnxt_free_def_cp_ring(struct bnxt *bp);
int bnxt_init_def_ring_struct(struct bnxt *bp, unsigned int socket_id);
void bnxt_handle_async_event(struct bnxt *bp, struct cmpl_base *cmp);
void bnxt_handle_fwd_req(struct bnxt *bp, struct cmpl_base *cmp);

#endif
