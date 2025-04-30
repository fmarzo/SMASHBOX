import firebase_admin
from firebase_admin import db, credentials
from serialpy.utils import update
from datetime import datetime

import config

class FirebaseDB:
    _instance = None
    _initialized = False

    def __new__(cls, *args, **kwargs):
        if cls._instance is None:
            cls._instance = super().__new__(cls)
        return cls._instance

    def __init__(self):
        if not self._initialized:
            FirebaseDB._initialized = True
            self.__cred = credentials.Certificate(config.DB_CREDENTIALS_PATH)
            self.__database_url = config.DB_DATABASE_URL
            self.__customer_list = list()
            self.__n_cust = 0
            self.db_init()

    def db_init(self):
        firebase_admin.initialize_app(self.__cred, {
            "databaseURL": self.__database_url})

        if not self.check_emptiness():
                self.update_customers_list()
                self.__n_cust = len(self.__customer_list)
                print("Stampo il numero di customers " + str(self.__n_cust))
        else:
            self.__n_cust = 0

    def update_customers_list(self):
        if not self.check_emptiness():
            self.__customer_list = db.reference(f"/Customers").get().items()
            self.__n_cust = len(self.__customer_list)
        else:
            self.__n_cust = 0

    def add_new_access(self, Box, Client):
        client = self.find_client(Client.client_id, Box.id)
        if client[0]:
            ref = self.get_db_reference().child(client[1])
            current_logs = ref.child("Logs").get()
            if current_logs is None:
                current_logs = []
            new_timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            current_logs.append(new_timestamp)
            ref.update({"Logs": current_logs})
            print(f"Log registered for {Client.client_id} at {new_timestamp}")
        else:
            print("Customer not found!")

    def get_logs(self, Box, Client) -> []:
        client = self.find_client(Client.client_id, Box.id)
        if client[0]:
            ref = self.get_db_reference().child(client[1])
            current_logs = ref.child("Logs").get()
            print (f"{current_logs} for {Client.client_id}")
            return current_logs
        else:
            print("Customer not found!")

    def get_db_reference (self):
        return db.reference(f"/Customers")

    def check_emptiness(self):
        if self.get_db_reference().get() is None:
            return True
        else:
            return False

    def fetch_customers_id(self):
        return self.__customer_list

    def insert_new_customer(self, Box, Client):
        print("Trying to insert a new customer..")
        client = self.find_client(Client.client_id,Box.id)
        if not client[0]:
            # This Customer does not exists, proceed with insertion
            new_item = {
                "Box": Box.id,
                "ID": Client.client_id,
                "Mail": Client.mail,
                "Name": Client.name,
                "Surname": Client.surname,
                "Logs": []
            }
            ref = db.reference(config.DB_ROOT_PATH)
            ref.push(new_item)
            self.update_customers_list()
        else:
            print("Customer exists already!")

    def     delete_customer(self, Box, Client):
        client = self.find_client(Client.client_id,Box.id)
        if client[0]:
            # Client Exists, proceed with deletion
            ref = self.get_db_reference()
            ref.child(client[1]).delete()
            print(f"Deleted client {Client.client_id} related to {Box.id}")
            self.update_customers_list()
        else:
            # No Customer found
            print("No Customer found! No Deletion")

    def update_customer(self, Box, Client, updates):
        client = self.find_client(Client.client_id,Box.id)
        if client[0]:
            # Client Exists, proceed with update
            ref = self.get_db_reference()
            ref.child(client[1]).update(updates)
            print(f"Updated client {Client.client_id} related to {Box.id}")
            self.update_customers_list()
        else:
            # No Customer found
            print("No Customer found! No Update")

    def find_client(self, id_client, id_box):
        if self.__n_cust != 0:
            for key, item in self.__customer_list:
                print (key)
                print(item)
                return True, key, item
        else:
            print("Customer list is empty!")
            return False, None, None











