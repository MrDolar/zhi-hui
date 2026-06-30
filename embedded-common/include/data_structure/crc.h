/**
 * @file crc.h
 * @brief CRC 校验算法
 */

#ifndef CRC_H
#define CRC_H

#include "common.h"

/* CRC8 */
uint8_t crc8(const uint8_t *data, size_t len);
uint8_t crc8_maxim(const uint8_t *data, size_t len);

/* CRC16 */
uint16_t crc16(const uint8_t *data, size_t len);
uint16_t crc16_ccitt(const uint8_t *data, size_t len);
uint16_t crc16_modbus(const uint8_t *data, size_t len);

/* CRC32 */
uint32_t crc32(const uint8_t *data, size_t len);

/* 校验和 */
uint8_t checksum8(const uint8_t *data, size_t len);
uint16_t checksum16(const uint8_t *data, size_t len);
uint32_t checksum32(const uint8_t *data, size_t len);

/* 异或校验 */
uint8_t xor_check(const uint8_t *data, size_t len);

/* LRC (纵向冗余校验) */
uint8_t lrc(const uint8_t *data, size_t len);

/* 查表法 CRC16 (快速) */
typedef struct {
    uint16_t table[256];
} crc16_table_t;

void crc16_init_table(crc16_table_t *ctx, uint16_t poly);
uint16_t crc16_calc(crc16_table_t *ctx, const uint8_t *data, size_t len);

#endif /* CRC_H */
