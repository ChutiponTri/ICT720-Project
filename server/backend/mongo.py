from pymongo import MongoClient
from dotenv import load_dotenv
import os

load_dotenv()

# Function to GET Database Connection
def get_db_connection():
    client = MongoClient(os.getenv("DB_ADDRESS"))
    return client

# Function to GET List of Database
def get_databases():
    client = get_db_connection()
    return client.list_database_names()

# Function to Insert a Data to Database
def insert_one(database, collection, data):
    client = get_db_connection()
    db = client[database]
    collection = db[collection]
    collection.insert_one(data)

# Function to GET All Value
def get_all_value(database, collection):
    client = get_db_connection()
    db = client[database]
    collection = db[collection]
    documents = collection.find()
    return list(documents)

# Function to GET Specific Data
def get_value_where(database, collection, query):
    client = get_db_connection()
    db = client[database]
    collection = db[collection]
    documents = collection.find(query)
    return list(documents)

# Function to GET Specific Field
def get_value_field(database, collection):
    client = get_db_connection()
    db = client[database]
    collection = db[collection]
    documents = collection.find({}, {"_id": 0, "name": 1, "email": 1})      # Retrieve documents with only the 'name' and 'email' fields
    return list(documents)

# Function to GET with Limit and Skip
def get_lim_skip(database, collection):
    client = get_db_connection()
    database = client[database]
    collection = database[collection]
    documents = collection.find().skip(2).limit(3)
    return list(documents)

# Function to GET with only one
def get_find_one(database, collection, data):
    client = get_db_connection()
    database = client[database]
    collection = database[collection]
    documents = collection.find_one(data)
    return documents

if __name__ == '__main__':
    database = "test_db"
    collection = "test"

    # insert_one(database, collection, {"name": "Yoyow", "email": "supawith.sans@gmail.com"})

    # result = get_value_where(database, collection, {"Hello": "Ton"})
    # print(result)

    # result = get_value_field(database, collection)
    # print(result)

    # result = get_lim_skip(database, collection)
    # print(result)

    result = get_find_one(database, collection, {"name": "Yoyow"})
    print(result)
