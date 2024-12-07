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
        self.update_customers_list()
        if self.__customer_list:
            self.__n_cust = len(self.__customer_list)
            print("Stampo il numero di customers " + str(self.__n_cust))

    def update_customers_list(self):
        self.__customer_list = db.reference(f"/Customers").get().items()

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
        ref = db.reference(f"/Customers")
        ref.push(new_item)

    def client_validity(self, id_client, id_box):
        self.update_customers_list()
        for key, item in self.__customer_list:
            if item.get('ID') == id_client:
                print (f"Found client ID {id_client}")
                if item.get('Box') == id_box:
                    print(f"Client ID {id_client} has Box {id_box}")
                    return
        print("No matches!")










