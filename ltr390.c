// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2023 Anshul Dalal <anshulusr@gmail.com>
 *
 * Driver for LiteOn LTR390 ALS and UV sensor
 *
 * Datasheet: https://optoelectronics.liteon.com/upload/download/DS86-2015-0004/LTR-390UV_Final_%20DS_V1%201.pdf
 */

#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/printk.h>

#define LTR390_DRV_NAME "ltr390"

static int ltr390_probe(struct i2c_client *client)
{
	return 0;
}

static const struct i2c_device_id ltr390_id[] = {
	{ LTR390_DRV_NAME, 0 },
	{ /* Sentinel */ },
};
MODULE_DEVICE_TABLE(i2c, ltr390_id);

static const struct of_device_id ltr390_of_match[] = {
	{
		.compatible = "liteon,ltr390",
	},
	{ /* Sentinel */ },
};
MODULE_DEVICE_TABLE(of, ltr390_of_match);

static struct i2c_driver ltr390_driver = {
	.driver = {
		.name = LTR390_DRV_NAME,
		.of_match_table = ltr390_of_match,
	},
	.probe = ltr390_probe,
	.id_table = ltr390_id,
};

module_i2c_driver(ltr390_driver);

MODULE_AUTHOR("Anshul Dalal <anshulusr@gmail.com>");
MODULE_DESCRIPTION("Adafruit Mini I2C Gamepad driver");
MODULE_LICENSE("GPL");
