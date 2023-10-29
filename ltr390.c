// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2023 Anshul Dalal <anshulusr@gmail.com>
 *
 * Driver for LiteOn LTR390 ALS and UV sensor
 *
 * Datasheet: https://optoelectronics.liteon.com/upload/download/DS86-2015-0004/LTR-390UV_Final_%20DS_V1%201.pdf
 */

#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/module.h>

#define LTR390_DRV_NAME "ltr390"

/* MAIN_CTRL Register */
#define LTR390_MAIN_CTRL 0x00
#define LTR390_SW_RESET 4
#define LTR390_UVS_MODE 3
#define LTR390_SENSOR_ENABLE 3

#define LTR390_PART_ID 0x06

static int ltr390_probe(struct i2c_client *client)
{
	char get_part_id[1] = { LTR390_PART_ID };
	u8 part_id;
	i2c_master_send(client, get_part_id, sizeof(get_part_id));
	i2c_master_recv(client, &part_id, sizeof(part_id));
	printk("Part ID: 0x%x\n", part_id);

	char software_reset[2] = { LTR390_MAIN_CTRL, BIT(LTR390_SW_RESET) };
	i2c_master_send(client, software_reset, sizeof(software_reset));

	/* Wait for the registers to reset before proceeding */
	mdelay(10);

	char enable_reading[2] = { LTR390_MAIN_CTRL,
				   BIT(LTR390_SENSOR_ENABLE) };
	i2c_master_send(client, enable_reading, sizeof(enable_reading));

	printk("Sensor init completed\n");

	for (int i = 0; i < 100; i++) {
		char get_data = LTR390_ALS_DATA;
		char received[3];
		i2c_master_send(client, &get_data, sizeof(get_data));
		i2c_master_recv(client, received, sizeof(received[0]));
		get_data++;
		i2c_master_send(client, &get_data, sizeof(get_data));
		i2c_master_recv(client, received + 1, sizeof(received[0]));
		get_data++;
		i2c_master_send(client, &get_data, sizeof(get_data));
		i2c_master_recv(client, received + 2, sizeof(received[0]));
		u32 data = received[0] | received[1] << 8 | received[2] << 16;
		printk("Data received: 0b%d[%x]\n", data, data);
		mdelay(100);
	}
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
