import firebase_admin
from firebase_admin import db, credentials
import config

class FirebaseDB:
    def __init__(self):
        self.__cred = credentials.Certificate(config.CREDENTIALS_PATH)
        self.__database_url = config.DATABASE_URL
        self.__customer_list = list()
        self.__n_cust = 0
        self.db_init()

    def db_init(self):
        firebase_admin.initialize_app(self.__cred, {
            "databaseURL": self.__database_url})
        cust_list = db.reference(f"/Customers").get()
        if cust_list:
            self.__n_cust = len(cust_list)
            print("Stampo il numero di customers")
            print(self.__n_cust)

        for id in range(self.__n_cust):
            ref = db.reference(f"/Customers/Client{id}/ID").get()
            self.__customer_list.append(ref)

    def fetch_customers_id(self):
        return self.__customer_list

    def insert_new_customer(self, Box, Client):
        #check if already exists
        new_item = {
            "Box": Box.id,
            "ID": Client.client_id,
            "Mail": Client.mail,
            "Name": Client.name,
            "Surname": Client.surname
        }
        print("Stampo il numero di customers" + str(self.__n_cust))
        ref = db.reference(f"/Customers/Client{self.__n_cust}")
        ref.push(new_item)

    def client_validity(self, id_client, id_box):
            id_list = self.fetch_customers_id()
            for id in id_list:
                if id == id_client:
                    id_box_assigned = db.reference(f"/Customers/Client{id}/Box").get()
                    if id_box_assigned == id_box:
                        return True
            return False





