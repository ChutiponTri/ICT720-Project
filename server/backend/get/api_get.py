from fastapi import APIRouter
from mongo import get_db_connection
import json
import os

router = APIRouter()

# Function to GET Specific Data
@router.get("/find")
async def get_value_where(query:str, collection:str):
    client = get_db_connection()
    db = client[os.getenv("DB_NAME")]
    collection = db[collection]
    documents = list(collection.find(json.loads(query), {"_id": 0}))
    return {"data": documents}

# Function to GET Specific Field
@router.get("/field")
async def get_value_field(query:str, collection:str):
    client = get_db_connection()
    db = client[os.getenv("DB_NAME")]
    collection = db[collection]
    documents = list(collection.find({"timestamp": {"$exists": True}}, {"_id": 0, "name": 1, "room": 1, "timestamp": 1}))
    return {"data": documents}

# Function to GET with only one
@router.get("/one")
async def get_find_one(query:str, collection:str, order:str="latest"):
    client = get_db_connection()
    database = client[os.getenv("DB_NAME")]
    collection = database[collection]
    sort_order = 1 if order == "latest" else -1
    documents = collection.find_one(json.loads(query), {"_id": 0}, sort=[("timestamp", sort_order)])
    return {"data": documents}

# Function to GET List of Database
@router.get("/db/list")
def get_databases():
    client = get_db_connection()
    return client.list_database_names()

# Function to GET 
@router.get("/users/")
async def read_users():
    return [{"username": "Rick"}, {"username": "Morty"}]

@router.get("/users/me")
async def read_user_me():
    return {"username": "fakecurrentuser"}

@router.get("/users/{username}")
async def read_user(username: str):
    return {"username": username}