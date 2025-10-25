Create an LV2 Host for Windows using native Windows APIs and C++.
Tasks:
- Using the mingw-w64 toolchain for compilation.
- Research LV2 specifications and requirements.
- Use Lilv library for LV2 plugin handling.
- I have added Lilv and its dependencies as a submodule in the project in the win32 directory.
- Use Win32 API for window management and audio handling.
- Implement plugin loading and management.
- Test with various LV2 plugins to ensure compatibility.
- Optimize performance for low-latency audio processing.
- Document the code and provide usage instructions.
- Explore integration with popular DAWs on Windows.
- Plan for future updates and maintenance.

User Interface:
- Design a simple and intuitive GUI using Win32 controls.
- Include features for plugin browsing, loading, and parameter adjustment.
- Top Header bar with Audio on / off toggle button.
- Plugin list panel on the left side.
- Main workspace area for plugin interaction and visualization.
- Status bar at the bottom for displaying messages and plugin info.
- Ensure responsiveness and usability across different screen resolutions.
- Structure the codebase for modularity and ease of maintenance.
- Organize code into separate modules for GUI, audio processing, and plugin management.
- Follow best practices for C++ coding standards and documentation.
