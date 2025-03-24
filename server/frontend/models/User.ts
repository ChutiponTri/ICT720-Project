import mongoose from "mongoose";

const UserSchema = new mongoose.Schema({
  username: { type: String, trim: true },
  email: { type: String, required: true, unique: true, trim: true, lowercase: true, match: [/.+@.+\..+/, "Invalid email format"] },
  clerkId: { type: String, required: true, unique: true },
  name: { type: String, trim: true },
  bio: { type: String, trim: true },
  image: { type: String },
  location: { type: String, trim: true },
  website: { type: String, trim: true }
  },
  { 
    timestamps: true,     // Enables createdAt and updatedAt
    collection: "users" 
  },
);

export default mongoose.models.User || mongoose.model("User", UserSchema);
