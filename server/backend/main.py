from fastapi import Depends, FastAPI
from get import api_get
from post import api_post

app = FastAPI()

app.include_router(api_get.router, prefix="/get", tags=["get"])
app.include_router(api_post.router, prefix="/post", tags=["post"])

@app.get("/")
async def root():
    return {"message": "ICT720: Development of Automation System for Wheelchair User"}