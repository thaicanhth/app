import 'package:app/pages/home_page.dart';
import 'package:app/pages/next_home.dart';
import 'package:flutter/material.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      title: "Smart Home",
      theme: ThemeData(
        fontFamily: "Poppins",
      ),
     home: Next_home(),
    );
  }
}
