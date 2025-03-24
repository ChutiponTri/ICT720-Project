# Use the official Node.js image (recommended for Next.js)
FROM node:18

# Set the working directory inside the container
WORKDIR /app

# Copy package.json and package-lock.json first (to optimize Docker caching)
COPY package.json package-lock.json ./

# Install dependencies
RUN npm install

# Copy the rest of the application files
COPY . .

# Expose Next.js port
EXPOSE 3000

# Command to run Next.js in development mode
# CMD ["npm", "run", "dev"]

# Uncomment below if you want to run in production mode instead
# RUN npm run build
# CMD ["npm", "start"]
