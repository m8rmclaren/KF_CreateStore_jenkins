"""
* FILE: main.py
* PROJECT: Create Cert Store in KF
* PURPOSE: Use Python requests library and data as arguments to create certificate store in Keyfactor
* AUTHOR: Hayden Roszell
* HISTORY: Version 2.0 11/30/2020
*
* Copyright © 2020 Keyfactor. All rights reserved.
"""

import requests
import datetime
import json
import sys


class Config:
    def __init__(self):
        self.cert_data = {}
        self.cert_data_lst = []
        with open("config.json", 'r') as datafile:
            self.serial = json.load(datafile) # open configuration file
        for i in range(len(sys.argv) - 1):
            self.cert_data_lst.append(sys.argv[i + 1]) # read in script arguments
        self.script_args = ["StoreName", "StoreType", "Path", "ContainerID", "Inventory"] # define arguments for dict
        self.zipped_cert_data = zip(self.script_args, self.cert_data_lst) # create iterator or tuples
        for field, data in self.zipped_cert_data:
            self.cert_data[field] = data # create dict of arguments

    def get_store_type(self):
        if self.cert_data["StoreType"] == "JKS":
            return 0
        elif self.cert_data["StoreType"] == "PEM":
            return 2


class Output:
    def __init__(self):
        self.store_id = str
        self.output_text = str
        self.log_file = "log.txt"
        self.output_file = "output.txt"
        self.timestamp = str
        self.get_timestamp()

    def get_timestamp(self):
        self.timestamp = datetime.datetime.utcnow().isoformat() + "Z"

    def write_to_file(self, option):
        timestamp = str(self.timestamp) + ": "
        if option == 1:
            write = timestamp + str(self.output_text)
            f = open(self.log_file, 'a')
        else:
            write = str(self.output_text)
            f = open(self.output_file, 'a')
        f.write(write + '\n')
        f.close()
        return

    def get_store_id(self, r):
        json_response = json.loads(json.dumps(r.json()))
        self.store_id = json_response['Id']

    def evaluate(self, r):
        if r.status_code == 200:
            self.output_text = "API call succeeded with status code " + str(r.status_code) + " OK"
            self.write_to_file(1)
        else:
            self.output_text = "API call failed with status code " + str(r.status_code)
            self.write_to_file(1)
            sys.exit(9)

    def evaluate_inv(self, r):
        if r.status_code == 204:
            self.output_text = "API call succeeded with status code " + str(r.status_code) + " OK"
            self.write_to_file(1)
        else:
            self.output_text = "API call failed with status code " + str(r.status_code)
            self.write_to_file(1)
            sys.exit(9)


def create_cert_store(output):
    config = Config()
    headers = {'authorization': config.serial["Auth"]["APIAuthorization"], 'Content-Type': 'application/json',
               'Accept': 'application/json',
               'x-keyfactor-requested-with': 'APIClient'}
    body = {
        "AgentId": config.serial["Misc"]["AgentID"],
        "ContainerId": config.cert_data["ContainerID"],
        "ClientMachine": config.serial["Misc"]["ClientName"],
        "AgentName": config.serial["Misc"]["ClientName"],
        "StorePath": config.cert_data["Path"] + config.cert_data["StoreName"],
        "CertStoreType": config.get_store_type(),
        "CreateIfMissing": "true",
        "Approved": "true",
        "Properties": "{\"ProviderType\":{\"updated\":\"true\",\"value\":\""+config.cert_data["StoreType"]+"\"}}"
    }
    r = requests.post(config.serial["URL"]["CreateStoreURL"], headers=headers, json=body)
    output.evaluate(r)
    output.get_store_id(r)
    return


def adjust_schedule(output):
    config = Config()
    headers = {'authorization': config.serial["Auth"]["APIAuthorization"], 'Content-Type': 'application/json',
               'Accept': 'application/json',
               'x-keyfactor-requested-with': 'APIClient'}
    body = {
        "StoreIds": [
            output.store_id
        ],
        "Schedule": {
            "Interval": {
                "Minutes": config.cert_data["Inventory"]
            }
        }
    }
    r = requests.post(config.serial["URL"]["ScheduleURL"], headers=headers, json=body)
    output.evaluate_inv(r)
    return


def main():
    output = Output()

    create_cert_store(output)
    adjust_schedule(output)


main()