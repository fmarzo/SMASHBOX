import firebase_admin
from firebase_admin import db, credentials

cred = credentials.Certificate("credential.json")
firebase_admin.initialize_app(cred, {"databaseURL" : "https://customerlist-970ff-default-rtdb.europe-west1.firebasedatabase.app/"})

for id in range (3):
    ref = db.reference(f"/Customers/Client{id}/ID").get()
    print(ref)

#print(ref.get("/Customers"))
