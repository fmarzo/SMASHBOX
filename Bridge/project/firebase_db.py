import firebase_admin
from firebase_admin import db, credentials
from google.auth.environment_vars import CREDENTIALS


DATABASE_URL = "https://customerlist-970ff-default-rtdb.europe-west1.firebasedatabase.app/"
CREDENTIALS_PATH = "credential.json"
class FirebaseDB:
    def __init__(self):
        self.__cred = credentials.Certificate(CREDENTIALS_PATH)
        self.__database_url = DATABASE_URL
        self.__customers_id = list()
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
            self.__customers_id.append(ref)

    def fetch_customers_id(self):
        return self.__customers_id

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





