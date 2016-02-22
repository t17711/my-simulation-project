====================
First Qt Application
====================

You need to create an application as shown on the slides.
It has several input widgets, some of which are in nested layouts.

After each step you should test your code. It helps to understand the influence your changes have.


Step 1: Create a main.cpp
=========================
 - Fill in your basic Qt code (see the hello world example earlier)
 - Don't forget to add the file to your .pro project file

Step 2: Create a top-level widget
=================================
 - Instantiate the top-level widget
 - Remember: Windows have no parent

Step 3: Create child widgets
============================

Instantiate multiple child widgets:
 - A label with text "Item:"
 - A line edit to enter items
 - A push button "Add"
 - A widget to display an item list

Note: Keep an eye on grouping your widgets in the same order as they appear on the GUI.

Step 4: Layout all widgets
==========================
 - Use the vertical and horizontal box layouts

Optional parts
==============
 - Provide a window title
 - Add "Edit", "Remove" buttons along the item list
 - Add a descriptive label to the item list
