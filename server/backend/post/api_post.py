from fastapi import APIRouter, HTTPException
from pydantic import BaseModel
from mongo import get_db_connection
from typing import Optional
from datetime import datetime
import os

router = APIRouter()

class Model(BaseModel):
    collection: str
    name: str
    home: str
    room: str
    status: Optional[str] = None
    timestamp: Optional[datetime] = None

@router.post("/data/insert")
def insert_one(data:Model):
    client = get_db_connection()
    database = client[os.getenv("DB_NAME")]
    collection = database[data.collection]
    exclude_fields = {"collection"}
    if data.collection == "room":
        exclude_fields.add("status")
    if data.collection == "status":
        if data.status is None:
            raise HTTPException(status_code=422, detail="Missing Wheelchair Status")
    data.timestamp = datetime.now()
    collection.insert_one(data.model_dump(exclude=exclude_fields))
    return {"Status": "Insert Successfully"}

