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

#ifndef _PMD_BNXT_H_
#define _PMD_BNXT_H_

#include <rte_ethdev.h>

/**
 * Enable/Disable tx loopback
 *
 * @param port
 *    The port identifier of the Ethernet device.
 * @param on
 *    1 - Enable tx loopback.
 *    0 - Disable tx loopback.
 *
 * @return
 *   - (0) if successful.
 *   - (-ENODEV) if *port* invalid.
 *   - (-EINVAL) if bad parameter.
 */
int rte_pmd_bnxt_set_tx_loopback(uint8_t port, uint8_t on);

/**
 * set all queues drop enable bit
 *
 * @param port
 *    The port identifier of the Ethernet device.
 * @param on
 *    1 - set the queue drop enable bit for all pools.
 *    0 - reset the queue drop enable bit for all pools.
 *
 * @return
 *   - (0) if successful.
 *   - (-ENODEV) if *port* invalid.
 *   - (-EINVAL) if bad parameter.
 */
int rte_pmd_bnxt_set_all_queues_drop_en(uint8_t port, uint8_t on);

/**
 * Set the VF MAC address.
 *
 * @param port
 *   The port identifier of the Ethernet device.
 * @param vf
 *   VF id.
 * @param mac_addr
 *   VF MAC address.
 * @return
 *   - (0) if successful.
 *   - (-ENODEV) if *port* invalid.
 *   - (-EINVAL) if *vf* or *mac_addr* is invalid.
 */
int rte_pmd_bnxt_set_vf_mac_addr(uint8_t port, uint16_t vf,
		struct ether_addr *mac_addr);

/**
 * Enable/Disable vf vlan strip for all queues in a pool
 *
 * @param port
 *    The port identifier of the Ethernet device.
 * @param vf
 *    ID specifying VF.
 * @param on
 *    1 - Enable VF's vlan strip on RX queues.
 *    0 - Disable VF's vlan strip on RX queues.
 *
 * @return
 *   - (0) if successful.
 *   - (-ENOTSUP) if hardware doesn't support this feature.
 *   - (-ENODEV) if *port* invalid.
 *   - (-EINVAL) if bad parameter.
 */
int
rte_pmd_bnxt_set_vf_vlan_stripq(uint8_t port, uint16_t vf, uint8_t on);

/**
 * Enable/Disable vf vlan insert
 *
 * @param port
 *    The port identifier of the Ethernet device.
 * @param vf
 *    ID specifying VF.
 * @param vlan_id
 *    0 - Disable VF's vlan insert.
 *    n - Enable; n is inserted as the vlan id.
 *
 * @return
 *   - (0) if successful.
 *   - (-ENODEV) if *port* invalid.
 *   - (-EINVAL) if bad parameter.
 */
int
rte_pmd_bnxt_set_vf_vlan_insert(uint8_t port, uint16_t vf,
		uint16_t vlan_id);

/**
 * Response sent back to bnxt driver from user app after callback
 */
enum rte_pmd_bnxt_mb_event_rsp {
	RTE_PMD_BNXT_MB_EVENT_NOOP_ACK,  /**< skip mbox request and ACK */
	RTE_PMD_BNXT_MB_EVENT_NOOP_NACK, /**< skip mbox request and NACK */
	RTE_PMD_BNXT_MB_EVENT_PROCEED,  /**< proceed with mbox request  */
	RTE_PMD_BNXT_MB_EVENT_MAX       /**< max value of this enum */
};

/* mailbox message types */
#define BNXT_VF_RESET			0x01 /* VF requests reset */
#define BNXT_VF_SET_MAC_ADDR	0x02 /* VF requests PF to set MAC addr */
#define BNXT_VF_SET_VLAN		0x03 /* VF requests PF to set VLAN */
#define BNXT_VF_SET_MTU			0x04 /* VF requests PF to set MTU */
#define BNXT_VF_SET_MRU			0x05 /* VF requests PF to set MRU */
/* TODO expose more */

/**
 * Data sent to the user application when the callback is executed.
 */
struct rte_pmd_bnxt_mb_event_param {
	uint16_t vf_id;     /**< Virtual Function number */
	int	retval;   /**< return value */
	void 	*msg;      /**< pointer to message */
};

/**
 * Enable/Disable VF MAC anti spoof
 *
 * @param port
 *    The port identifier of the Ethernet device.
 * @param vf
 *   VF id.
 * @param on
 *    1 - Enable VF MAC anti spoof.
 *    0 - Disable VF MAC anti spoof.
 *
 * @return
 *   - (0) if successful.
 *   - (-ENODEV) if *port* invalid.
 *   - (-EINVAL) if bad parameter.
 */
int rte_pmd_bnxt_set_vf_mac_anti_spoof(uint8_t port, uint16_t vf, uint8_t on);

/**
 * Get VF's statistics
 *
 * @param port
 *    The port identifier of the Ethernet device.
 * @param vf_id
 *    VF on which to get.
 * @param stats
 *    A pointer to a structure of type *rte_eth_stats* to be filled with
 *    the values of device counters supported statistics:
 * @return
 *   - (0) if successful.
 *   - (-ENODEV) if *port* invalid.
 *   - (-EINVAL) if bad parameter.
 */

int rte_pmd_bnxt_get_vf_stats(uint8_t port,
			      uint16_t vf_id,
			      struct rte_eth_stats *stats);

/**
 * Clear VF's statistics
 *
 * @param port
 *    The port identifier of the Ethernet device.
 * @param vf_id
 *    VF on which to get.
 * @return
 *   - (0) if successful.
 *   - (-ENODEV) if *port* invalid.
 *   - (-EINVAL) if bad parameter.
 */
int rte_pmd_bnxt_reset_vf_stats(uint8_t port,
				uint16_t vf_id);

/**
 * Enable/Disable VF VLAN anti spoof
 *
 * @param port
 *    The port identifier of the Ethernet device.
 * @param vf
 *   VF id.
 * @param on
 *    1 - Enable VF VLAN anti spoof.
 *    0 - Disable VF VLAN anti spoof.
 *
 * @return
 *   - (0) if successful.
 *   - (-ENODEV) if *port* invalid.
 *   - (-EINVAL) if bad parameter.
 */
int rte_pmd_bnxt_set_vf_vlan_anti_spoof(uint8_t port, uint16_t vf, uint8_t on);

/**
 * Returns the number of default RX queues on a VF
 *
 * @param port
 *    The port identifier of the Ethernet device.
 * @param vf
 *   VF id.
 * @return
 *   - Non-negative value - Number of default RX queues
 *   - (-EINVAL) if bad parameter.
 *   - (-ENOTSUP) if on a function without VFs
 *   - (-ENOMEM) on an allocation failure
 *   - (-1) firmware interface error
 */
int rte_pmd_bnxt_get_vf_rx_status(uint8_t port, uint16_t vf_id);

#endif /* _PMD_BNXT_H_ */
