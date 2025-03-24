# Use an Alpine image with Python installed
FROM python:3.10-slim

# Set the working directory inside the container
WORKDIR /app

# Copy the local files to the container
COPY requirements.txt requirements.txt

# Install Streamlit and other required Python packages (if there's a requirements.txt)
RUN pip install -U pip
RUN pip install -r requirements.txt

# Copy the local files to the container
COPY . .

# Command to run the Streamlit app -> If Compose Comment This Line
# CMD ["fastapi", "run", "main.py", "--port", "8305"]           # I change port 80 to 8305
