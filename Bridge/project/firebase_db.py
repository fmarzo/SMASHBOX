import firebase_admin
from firebase_admin import db, credentials
import config

class FirebaseDB:
    def __init__(self):
        self.__cred = credentials.Certificate(config.DB_CREDENTIALS_PATH)
        self.__database_url = config.DB_DATABASE_URL
        self.__customer_list = list()
        self.__n_cust = 0
        self.db_init()

    def db_init(self):
        firebase_admin.initialize_app(self.__cred, {
            "databaseURL": self.__database_url})
        if self.__customer_list:
            self.__n_cust = len(self.__customer_list)
            print("Stampo il numero di customers " + str(self.__n_cust))

    def update_customers_list(self):
        self.__customer_list = db.reference(f"/Customers").get().items()
        self.__n_cust = len(self.__customer_list)

    def get_db_reference (self):
        return db.reference(f"/Customers")

    def fetch_customers_id(self):
        return self.__customer_list

    def insert_new_customer(self, Box, Client):
        print("Trying to insert a new customer..")
        client = self.find_client(Client.client_id,Box.id)
        if not client[0] & self.__n_cust != 0:
            new_item = {
                "Box": Box.id,
                "ID": Client.client_id,
                "Mail": Client.mail,
                "Name": Client.name,
                "Surname": Client.surname
            }
            ref = db.reference(config.DB_ROOT_PATH)
            ref.push(new_item)
            self.update_customers_list()
        else:
            print("Customer exists already!")

    def delete_customer(self, Box, Client):
        client = self.find_client(Client.client_id,Box.id)
        if client[0]:
            ref = self.get_db_reference()
            ref.child(client[1]).delete()
            print(f"Deleted client {Client.client_id} related to {Box.id}")
            self.update_customers_list()
        else:
            print("No Deletion!")

    def find_client(self, id_client, id_box):
        if self.__n_cust != 0:
            for key, item in self.__customer_list:
                if item.get(config.DB_FIELD_ID) == id_client:
                    if item.get(config.DB_FIELD_BOX) == id_box:
                        print ("Found Customers" + "Name " + item.get(config.DB_FIELD_NAME) +
                               f"Surname " + item.get(config.DB_FIELD_SURNAME) +
                               f"Client ID {id_client} has Box {id_box}")
                        return True, key, item
            print("No matches!")
            return False, None, None
        else:
            print("Customer list empty!")
            return False, None, None

    #TODO: Add routine to query clients trough client_id
    #TODO: Add routine to query clients trough box_id











