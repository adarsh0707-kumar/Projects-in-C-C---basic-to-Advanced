/**
 * @file memory.h
 * @brief Single-register calculator memory (MS/MR/MC/M+/M-).
 */
#ifndef MEMORY_H
#define MEMORY_H

/** @brief Stores a value into memory (MS), overwriting any previous value. */
void memoryStore(double value);
/** @brief Returns the current value in memory (MR). */
double memoryRecall(void);
/** @brief Adds a value to memory (M+). */
void memoryAdd(double value);
/** @brief Subtracts a value from memory (M-). */
void memorySubtract(double value);
/** @brief Resets memory to 0 (MC). */
void memoryClear(void);

#endif
