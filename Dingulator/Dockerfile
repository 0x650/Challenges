FROM nginx:alpine

# Copy only the built React app to nginx's html directory
COPY dist /usr/share/nginx/html

# Expose port 80
EXPOSE 80

# Start nginx
CMD ["nginx", "-g", "daemon off;"]


