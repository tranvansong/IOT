package com.example.demo.controller;

import java.util.Arrays;
import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.messaging.handler.annotation.MessageMapping;
import org.springframework.messaging.handler.annotation.SendTo;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.client.RestTemplate;
import org.springframework.web.servlet.mvc.support.RedirectAttributes;

import com.example.demo.entity.Admin;
import com.example.demo.model.MqttSubscribeModel;
import com.example.demo.service.AdminService;

import jakarta.servlet.http.Cookie;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpSession;

@Controller
public class MainController {
	
	@Autowired
	private AdminService adminService;

	@GetMapping("/login")
	public String loginPage() {
		return "login";
	}
	
	@RequestMapping(value = "/checkLogin", method = RequestMethod.POST)
	public String checkLogin(@RequestParam("username") String username, @RequestParam("password") String password, RedirectAttributes redirectAttributes, HttpSession session) {
		Admin admin = adminService.checkAdmin(username, password);
		System.out.println(admin);
		if(admin == null) {
			redirectAttributes.addFlashAttribute("error", "Username hoặc password không đúng");
			return "redirect:/login";
		}
		session.setAttribute("admin", admin);
		return "redirect:/dashboard";
		
	}

	@GetMapping("/logout")
	public String logout(HttpServletRequest request, RedirectAttributes redirectAttributes) {
		 HttpSession session = request.getSession(false);
        if (session != null) {
            session.invalidate();
        }
        redirectAttributes.addFlashAttribute("logout", "Bạn đã đăng xuất vui lòng đăng nhập lại");
		return "redirect:/login";
	}
	
	
	@GetMapping("/dashboard")
	public String dashboardPage(Model model, HttpSession session) {
		Admin admin = (Admin) session.getAttribute("admin");
		
		if(admin == null) {
			return "redirect:/login";
		}
		model.addAttribute("admin", admin);
		return "dashboard";
	}
	
	@GetMapping("/alert-history")
	public String alertHistoryPage(Model model, HttpSession session) {
		Admin admin = (Admin) session.getAttribute("admin");
		
		if(admin == null) {
			return "redirect:/login";
		}
		model.addAttribute("admin", admin);
		return "alert-history";
	}
}
