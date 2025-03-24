"use server";

import connectToDatabase from "@/lib/mongodb";
import { auth, currentUser } from "@clerk/nextjs/server";
import User from "../../models/User";

export async function syncUser() {
  try {
    const { userId } = await auth();
    const user = await currentUser();
    await connectToDatabase();
    
    if (!userId || !user) return;

    try {
      const existingUser = await User.findOne({ clerkId: userId });

      if (existingUser) return existingUser;

      const dbUser = await User.create({
        clerkId: userId,
        name: `${user.firstName || ""} ${user.lastName || ""}`.trim(),
        username: user.username ?? user.emailAddresses[0].emailAddress.split("@")[0],
        email: user.emailAddresses[0].emailAddress,
        image: user.imageUrl,
      });

      return dbUser;
    } catch (error) {
      console.log("error", error);
    }
  } catch (error) {
    console.log("Error in syncUser", error);
  }
}
